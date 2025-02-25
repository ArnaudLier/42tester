use clap::Parser;
use indicatif::{ProgressBar, ProgressStyle};
use rand_unique::{RandomSequence, RandomSequenceBuilder};
use std::{
    fs::OpenOptions,
    io::{Read, Write},
    num::{NonZeroU32, NonZeroUsize},
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
    objective: Option<u32>,
) {
    let mut rng = rand::thread_rng();
    let config = RandomSequenceBuilder::<u32>::rand(&mut rng);
    let sequence: RandomSequence<u32> = config.into_iter();
    // Only test separate arguments because that's what the subject requires.
    let args = sequence
        .take(number_count)
        .map(|i| (i as i32).to_string())
        .collect::<Vec<String>>();
    let joined_args = args.join(" ");

    let mut push_swap: std::process::Child = Command::new(PUSH_SWAP_PATH)
        .args(&args)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .expect("failed to execute push_swap");
    let mut checker = Command::new(checker_path)
        .args(&args)
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
        eprintln!("{RED}{joined_args} errored with {PUSH_SWAP_PATH}{RESET}");
    }

    if checker
        .stdin
        .take()
        .expect("failed to open checker stdin")
        .write_all(push_swap_stdout.as_bytes())
        .is_err()
    {
        eprintln!("{RED}failed to write all instructions to checker...{RESET}");
    }

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
    if checker_stdout != "OK\n" {
        eprintln!("{RED}{joined_args} didn't pass checker: {checker_stdout:?}{RESET}");
    }

    // TODO: warn on inefficient double rotations
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
    if let Some(objective) = objective {
        if instruction_count > objective {
            let mut file = OpenOptions::new()
                .append(true)
                .create(true)
                .open(format!("{objective}.rejected"))
                .expect("couldn't open rejected file");
            file.write_all(format!("{joined_args}\n").as_bytes())
                .expect("couldn't write rejected to file");
        }
    }
    push_swap.wait().unwrap();
    checker.wait().unwrap();
    bar.inc(1);
}

fn test_batch(
    test_count: NonZeroU32,
    number_count: usize,
    objective: Option<u32>,
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
            "[{elapsed_precise}] {bar:40.cyan/blue} {pos:>7}/{len:7} {msg} {eta_precise}",
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
            test(
                bar,
                number_count,
                sum,
                minimum,
                maximum,
                checker_path,
                objective,
            );
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
    if let Some(objective) = objective {
        if *maximum.lock().unwrap() > objective {
            print!("{RED}");
        } else {
            print!("{GREEN}");
        }
        println!("Objective: <= {} instructions{RESET}", objective);
    }
    println!();
}

fn validate_args(
    program_path: &str,
    args: &[&str],
    stdin: Option<&str>,
    expected_stdout: &str,
    expected_stderr: &str,
) {
    let mut program = Command::new(program_path)
        .args(args)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .unwrap_or_else(|_| panic!("failed to execute {program_path}"));

    let mut program_stdout = String::new();
    let mut program_stderr = String::new();
    if let Some(stdin) = stdin {
        program
            .stdin
            .take()
            .unwrap_or_else(|| panic!("couldn't open {program_path} stdin"))
            .write_all(stdin.as_bytes())
            .unwrap_or_else(|_| panic!("couldn't write to {program_path} stdin"));
    }
    program
        .stdout
        .take()
        .expect("failed to open program stdout")
        .read_to_string(&mut program_stdout)
        .expect("failed to read program stdout");
    program
        .stderr
        .take()
        .expect("failed to open program stderr")
        .read_to_string(&mut program_stderr)
        .expect("failed to read program stderr");
    program.wait().unwrap();
    if program_stdout != expected_stdout {
        eprintln!("{RED}{program_path} doesn't give expected stdout of {expected_stdout:?} (got {program_stdout:?}) with args {args:?} and stdin of {stdin:?}{RESET}")
    }
    if program_stderr != expected_stderr {
        eprintln!("{RED}{program_path} doesn't give expected stderr of {expected_stderr:?} (got {program_stderr:?}) with args {args:?} and stdin of {stdin:?}{RESET}")
    }
}

const ERROR_OUTPUT: &str = "Error\n";
fn test_args(program_path: &str) {
    validate_args(program_path, &[], None, "", "");
    validate_args(program_path, &["1", "1"], None, "", ERROR_OUTPUT);
    validate_args(program_path, &["1", "1e"], None, "", ERROR_OUTPUT);
    validate_args(program_path, &["1", "2147483648"], None, "", ERROR_OUTPUT);
    validate_args(
        program_path,
        &["1", "99999999999999"],
        None,
        "",
        ERROR_OUTPUT,
    );
    validate_args(
        program_path,
        &["1", "18446744073709551564"],
        None,
        "",
        ERROR_OUTPUT,
    );
    validate_args(program_path, &["1", "-2147483649"], None, "", ERROR_OUTPUT);
    validate_args(
        program_path,
        &["1", "-21474836490000"],
        None,
        "",
        ERROR_OUTPUT,
    );
}

#[derive(Parser, Debug)]
#[command(version, about)]
struct Args {
    #[arg(short, long)]
    test_count: Option<NonZeroU32>,
    #[arg(short, long)]
    number_count: Option<NonZeroUsize>,
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
        eprintln!("{RED}{checker_path} doesn't exist in current working directory.{RESET}");
        return;
    }
    if !Path::new(PUSH_SWAP_PATH).exists() {
        eprintln!("{RED}{PUSH_SWAP_PATH} doesn't exist in current working directory.{RESET}");
        return;
    }
    if let Some(number_count) = args.number_count {
        test_batch(test_count, number_count.get(), None, checker_path);
    } else {
        test_batch(test_count, 1, Some(0), checker_path);
        test_batch(test_count, 2, Some(1), checker_path);
        test_batch(test_count, 3, Some(3), checker_path);
        test_batch(test_count, 5, Some(12), checker_path);
        test_batch(test_count, 100, Some(700), checker_path);
        test_batch(test_count, 500, Some(5500), checker_path);
    }
    test_args(PUSH_SWAP_PATH);
    test_args(checker_path);
    validate_args(checker_path, &["2", "1"], Some("sa"), "", ERROR_OUTPUT);
    validate_args(checker_path, &["2", "1"], Some("rr\n"), "OK\n", "");
}
