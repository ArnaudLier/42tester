use clap::Parser;
use std::{
    io::{Read, Write},
    path::Path,
    process::{Command, Stdio},
    sync::{Arc, Mutex},
};

use rand::Rng;
use threadpool::ThreadPool;

const RED: &str = "\x1B[1;31m";
const GREEN: &str = "\x1B[1;32m";
const RESET: &str = "\x1B[0m";

fn test(
    number_count: u32,
    sum: Arc<Mutex<f64>>,
    minimum: Arc<Mutex<u32>>,
    maximum: Arc<Mutex<u32>>,
    use_own_checker: bool,
) {
    let mut rng = rand::thread_rng();
    let arg = (0..number_count)
        .map(|_| rng.gen_range(i32::MIN..=i32::MAX))
        .map(|i| i.to_string())
        .collect::<Vec<String>>()
        .join(" ");

    let mut push_swap = Command::new("./push_swap")
        .arg(&arg)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .expect("failed to execute push_swap");
    let mut program = "./checker_linux";
    if use_own_checker {
        program = "./checker";
    }
    let mut checker = Command::new(program)
        .arg(&arg)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .expect("failed to execute checker");
    let mut push_swap_stdout = String::new();
    let mut push_swap_stderr = String::new();
    push_swap
        .stdout
        .take()
        .expect("failed to open push_swap stdout")
        .read_to_string(&mut push_swap_stdout)
        .expect("failed to read push_swap stdout");
    push_swap
        .stderr
        .take()
        .expect("failed to open push_swap stderr")
        .read_to_string(&mut push_swap_stderr)
        .expect("failed to read push_swap stderr");
    if push_swap_stderr.contains("Error") {
        eprintln!("{RED}{arg} failed...{RESET}");
    }
    checker
        .stdin
        .take()
        .expect("failed to open checker stdin")
        .write_all(push_swap_stdout.as_bytes())
        .expect("failed to write to checker stdin");
    let instruction_count = push_swap_stdout.lines().count() as u32;
    *sum.lock().unwrap() += f64::from(instruction_count);
    let mut minimum: std::sync::MutexGuard<'_, u32> = minimum.lock().unwrap();
    let mut maximum = maximum.lock().unwrap();
    if instruction_count > *maximum {
        *maximum = instruction_count;
    }
    if instruction_count < *minimum {
        *minimum = instruction_count;
    }
    push_swap.wait().unwrap();
    checker.wait().unwrap();
}

fn test_batch(test_count: u32, number_count: u32, objective: u32, use_own_checker: bool) {
    println!("Testing {number_count} random numbers {test_count} time(s)...");
    let pool = ThreadPool::new(12);
    let sum = Arc::new(Mutex::new(0.0));
    let minimum = Arc::new(Mutex::new(u32::MAX));
    let maximum = Arc::new(Mutex::new(0u32));
    for _ in 0..test_count {
        let sum = Arc::clone(&sum);
        let minimum = Arc::clone(&minimum);
        let maximum = Arc::clone(&maximum);
        pool.execute(move || {
            test(number_count, sum, minimum, maximum, use_own_checker);
        });
    }
    pool.join();

    println!("Minimum: {} instructions", *minimum.lock().unwrap());
    println!(
        "Average: {} instructions",
        *sum.lock().unwrap() / test_count as f64
    );
    println!("Maximum: {} instructions", *maximum.lock().unwrap());
    if *maximum.lock().unwrap() > objective {
        print!("{RED}");
    } else {
        print!("{GREEN}");
    }
    println!("Objective: <= {} instructions{RESET}", objective);
    println!();
}

#[derive(Parser, Debug)]
#[command(version, about)]
struct Args {
    #[arg(short, long)]
    test_count: Option<u32>,
    #[arg(short, long)]
    use_own_checker: bool,
}

fn main() {
    let args = Args::parse();
    let test_count = args.test_count.unwrap_or(100);
    if test_count == 0 {
        eprintln!("Zero is not allowed");
        return;
    }
    if !Path::new("./checker").exists() {
        eprintln!("Checker doesn't exist");
        return;
    }
    if !Path::new("./push_swap").exists() {
        eprintln!("Push swap doesn't exist");
        return;
    }
    test_batch(test_count, 1, 0, args.use_own_checker);
    test_batch(test_count, 2, 1, args.use_own_checker);
    test_batch(test_count, 3, 3, args.use_own_checker);
    test_batch(test_count, 5, 12, args.use_own_checker);
    test_batch(test_count, 100, 700, args.use_own_checker);
    test_batch(test_count, 500, 5500, args.use_own_checker);
}
