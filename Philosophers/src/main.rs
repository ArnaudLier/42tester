use clap::Parser;
use indicatif::{ProgressBar, ProgressStyle};
use std::{
    io::Read,
    num::{NonZeroU32, NonZeroU8},
    path::Path,
    process::{Command, Stdio},
    sync::{Arc, Mutex},
};

use threadpool::ThreadPool;

const RED: &str = "\x1B[1;31m";
const GREEN: &str = "\x1B[1;32m";
const RESET: &str = "\x1B[0m";
const PHILO_PATH: &str = "./philo";

fn test(
    bar: Arc<ProgressBar>,
    config: PhilosopherConfig,
    expectations: PhilosopherExpectations,
    failed: Arc<Mutex<Vec<FailedExpectation>>>,
) {
    let args = if let Some(stop_at_meals) = config.stop_at_meals {
        vec![
            config.philosopher_count.to_string(),
            config.time_to_die.to_string(),
            config.time_to_eat.to_string(),
            config.time_to_sleep.to_string(),
            stop_at_meals.to_string(),
        ]
    } else {
        vec![
            config.philosopher_count.to_string(),
            config.time_to_die.to_string(),
            config.time_to_eat.to_string(),
            config.time_to_sleep.to_string(),
        ]
    };

    let mut philo: std::process::Child = Command::new(PHILO_PATH)
        .args(args)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .expect("failed to execute philo");

    let mut philo_stdout = String::new();
    let mut philo_stderr = String::new();
    philo
        .stdout
        .take()
        .expect("failed to open philo stdout")
        .read_to_string(&mut philo_stdout)
        .expect("failed to read philo stdout");
    philo
        .stderr
        .take()
        .expect("failed to open philo stderr")
        .read_to_string(&mut philo_stderr)
        .expect("failed to read philo stderr");
    philo.wait().unwrap();
    let mut dead = false;
    for line in philo_stdout.lines() {
        if dead {
            eprintln!("{philo_stdout:?}\n");
            (*failed.lock().unwrap()).push(FailedExpectation::MessageAfterDeath);
        } else if line.contains("died") || line.contains("die") {
            dead = true;
        }
    }
    if !philo_stderr.is_empty() {
        eprintln!("{RED}received something in stderr from {PHILO_PATH}{RESET}");
    }
    if dead && !expectations.should_die {
        (*failed.lock().unwrap()).push(FailedExpectation::ShouldNotDie);
    } else if !dead && expectations.should_die {
        (*failed.lock().unwrap()).push(FailedExpectation::ShouldDie);
    }

    bar.inc(1);
}

#[derive(Debug)]
enum FailedExpectation {
    ShouldDie,
    ShouldNotDie,
    MessageAfterDeath,
}

fn test_batch(
    concurrency: NonZeroU8,
    test_count: NonZeroU32,
    config: PhilosopherConfig,
    expectations: PhilosopherExpectations,
) {
    println!(
        "Testing {config:?} {expectations:?} {test_count} time(s) (concurrency {concurrency})..."
    );
    let failed_expectations: Arc<Mutex<Vec<FailedExpectation>>> = Arc::new(Mutex::new(vec![]));
    let pool = ThreadPool::new(concurrency.get() as usize);
    let bar = Arc::new(ProgressBar::new(test_count.get() as u64));
    bar.set_style(
        ProgressStyle::with_template(
            "[{elapsed_precise}] {bar:40.cyan/blue} {pos:>7}/{len:7} {msg} {eta_precise}",
        )
        .unwrap()
        .progress_chars("##-"),
    );
    for _ in 0..test_count.get() {
        let bar = Arc::clone(&bar);
        let failed_expectations = Arc::clone(&failed_expectations);
        let config = config.clone();
        let expectations = expectations.clone();
        pool.execute(move || {
            test(bar, config, expectations, failed_expectations);
        });
    }
    pool.join();
    bar.finish();

    let failed_expectations = failed_expectations.lock().unwrap();
    if failed_expectations.is_empty() {
        println!("{GREEN}Congrats!{RESET}");
    } else {
        eprintln!("{RED}{failed_expectations:?}{RESET}");
    }
    println!();
}

#[derive(Parser, Debug)]
#[command(version, about)]
struct Args {
    #[arg(short, long)]
    test_count: Option<NonZeroU32>,
    /// Max tests done concurrently
    #[arg(short, long)]
    concurrency: Option<NonZeroU8>,
}

#[derive(Debug, Clone)]
struct PhilosopherConfig {
    philosopher_count: u8,
    time_to_die: u32,
    time_to_eat: u32,
    time_to_sleep: u32,
    stop_at_meals: Option<u32>,
}

#[derive(Debug, Clone)]
struct PhilosopherExpectations {
    should_die: bool,
}

fn main() {
    let args = Args::parse();
    let test_count = args.test_count.unwrap_or(100.try_into().unwrap());
    let concurrency = args.concurrency.unwrap_or(10.try_into().unwrap());

    if !Path::new(PHILO_PATH).exists() {
        eprintln!("{RED}{PHILO_PATH} doesn't exist in current working directory.{RESET}");
        return;
    }
    let tests = vec![
        (
            PhilosopherConfig {
                philosopher_count: 2,
                time_to_die: 810,
                time_to_eat: 200,
                time_to_sleep: 200,
                stop_at_meals: Some(10),
            },
            PhilosopherExpectations { should_die: false },
        ),
        (
            PhilosopherConfig {
                philosopher_count: 1,
                time_to_die: 200,
                time_to_eat: 200,
                time_to_sleep: 100,
                stop_at_meals: None,
            },
            PhilosopherExpectations { should_die: true },
        ),
        (
            PhilosopherConfig {
                philosopher_count: 3,
                time_to_die: 400,
                time_to_eat: 200,
                time_to_sleep: 200,
                stop_at_meals: None,
            },
            PhilosopherExpectations { should_die: true },
        ),
        (
            PhilosopherConfig {
                philosopher_count: 5,
                time_to_die: 800,
                time_to_eat: 200,
                time_to_sleep: 200,
                stop_at_meals: Some(7),
            },
            PhilosopherExpectations { should_die: false },
        ),
        (
            PhilosopherConfig {
                philosopher_count: 4,
                time_to_die: 410,
                time_to_eat: 200,
                time_to_sleep: 200,
                stop_at_meals: Some(7),
            },
            PhilosopherExpectations { should_die: false },
        ),
        (
            PhilosopherConfig {
                philosopher_count: 200,
                time_to_die: 800,
                time_to_eat: 200,
                time_to_sleep: 200,
                stop_at_meals: Some(7),
            },
            PhilosopherExpectations { should_die: false },
        ),
        (
            PhilosopherConfig {
                philosopher_count: 5,
                time_to_die: 20,
                time_to_eat: 1,
                time_to_sleep: 1,
                stop_at_meals: Some(100),
            },
            PhilosopherExpectations { should_die: false },
        ),
    ];

    for test in tests {
        test_batch(concurrency, test_count, test.0, test.1);
    }
}
