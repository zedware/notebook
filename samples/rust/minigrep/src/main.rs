use std::env;
use std::process;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().skip(1).collect();
    dbg!(&args);
    let (query, file) = parse_args(&args);
    grep_file(query, file);
}

fn parse_args(args: &[String]) -> (&str, &str) {
    if args.len() != 2 {
        eprintln!("Arguments not equal to 2!");
        process::exit(1);
    }

    let query = &args[0];
    let file = &args[1];
    (query, file)
}

fn grep_file(query: &str, file: &str) -> () {
    println!("Search for: {query}\n");
    let contents = fs::read_to_string(file)
        .expect("Should have been able to read the file");
    println!("With text:\n{contents}");
}
