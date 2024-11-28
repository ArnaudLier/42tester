use clap::Parser;
use indicatif::{ProgressBar, ProgressStyle};
use rand_unique::{RandomSequence, RandomSequenceBuilder};
use std::{
    io::{Read, Write},
    num::NonZeroU32,
    path::Path,
    process::{Command, Stdio},
    sync::{Arc, Mutex},
};

use threadpool::ThreadPool;

const RED: &str = "\x1B[1;31m";
const GREEN: &str = "\x1B[1;32m";
const RESET: &str = "\x1B[0m";
const PUSH_SWAP_PATH: &str = "./push_swap";

fn test(
    bar: Arc<ProgressBar>,
    number_count: usize,
    sum: Arc<Mutex<f64>>,
    minimum: Arc<Mutex<u32>>,
    maximum: Arc<Mutex<u32>>,
    checker_path: &str,
) {
    let mut rng = rand::thread_rng();
    let config = RandomSequenceBuilder::<u32>::rand(&mut rng);
    let sequence: RandomSequence<u32> = config.into_iter();
    let arg = sequence
        .take(number_count)
        .map(|i| (i as i32).to_string())
        .collect::<Vec<String>>()
        .join(" ");

    let mut push_swap: std::process::Child = Command::new(PUSH_SWAP_PATH)
        .arg(&arg)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .expect("failed to execute push_swap");
    let mut checker = Command::new(checker_path)
        .arg(&arg)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
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
    if push_swap_stderr == "Error\n" {
        eprintln!("{RED}{arg} failed...{RESET}");
    }

    checker
        .stdin
        .take()
        .expect("failed to open checker stdin")
        .write_all(push_swap_stdout.as_bytes())
        .expect("failed to write to checker stdin");

    let mut checker_stdout = String::new();
    let mut checker_stderr = String::new();
    checker
        .stdout
        .take()
        .expect("failed to open checker stdout")
        .read_to_string(&mut checker_stdout)
        .expect("failed to read checker stdout");
    checker
        .stderr
        .take()
        .expect("failed to open checker stderr")
        .read_to_string(&mut checker_stderr)
        .expect("failed to read checker stderr");
    if checker_stdout == "Error\n" || checker_stdout == "KO\n" {
        eprintln!("{RED}{arg} failed...{RESET}");
    }

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
    bar.inc(1);
}

fn test_batch(
    test_count: NonZeroU32,
    number_count: usize,
    objective: u32,
    checker_path: &'static str,
) {
    println!("Testing {number_count} random numbers {test_count} time(s)...");
    let pool = ThreadPool::new(12);
    let sum = Arc::new(Mutex::new(0.0));
    let minimum = Arc::new(Mutex::new(u32::MAX));
    let maximum = Arc::new(Mutex::new(0u32));
    let bar = Arc::new(ProgressBar::new(test_count.get() as u64));
    bar.set_style(
        ProgressStyle::with_template(
            "[{elapsed_precise}] {bar:40.cyan/blue} {pos:>7}/{len:7} {msg} {eta}",
        )
        .unwrap()
        .progress_chars("##-"),
    );
    for _ in 0..test_count.get() {
        let sum = Arc::clone(&sum);
        let minimum = Arc::clone(&minimum);
        let maximum = Arc::clone(&maximum);
        let bar = Arc::clone(&bar);
        pool.execute(move || {
            test(bar, number_count, sum, minimum, maximum, checker_path);
        });
    }
    pool.join();
    bar.finish();

    println!("Minimum: {} instructions", *minimum.lock().unwrap());
    println!(
        "Average: {} instructions",
        *sum.lock().unwrap() / test_count.get() as f64
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
    test_count: Option<NonZeroU32>,
    #[arg(short, long)]
    use_own_checker: bool,
}

fn main() {
    let args = Args::parse();
    let test_count = args.test_count.unwrap_or(100.try_into().unwrap());
    let mut checker_path: &'static str = if cfg!(target_os = "linux") {
        "./checker_linux"
    } else {
        "./checker_Mac"
    };
    if args.use_own_checker {
        checker_path = "./checker";
    }
    if !Path::new(checker_path).exists() {
        eprintln!("{RED}{checker_path} doesn't exist.{RESET}");
        return;
    }
    if !Path::new(PUSH_SWAP_PATH).exists() {
        eprintln!("{RED}{PUSH_SWAP_PATH} doesn't exist.{RESET}");
        return;
    }
    test_batch(test_count, 1, 0, checker_path);
    test_batch(test_count, 2, 1, checker_path);
    test_batch(test_count, 3, 3, checker_path);
    test_batch(test_count, 5, 12, checker_path);
    test_batch(test_count, 100, 700, checker_path);
    test_batch(test_count, 500, 5500, checker_path);
}
