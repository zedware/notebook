use std::env;
use std::process;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().skip(1).collect();
    dbg!(&args);
    let config = Config::new(&args).unwrap_or_else(|err| {
        println!("Problem parsing arguments: {err}");
        process::exit(1);
    });
    grep_file(config);
}

struct Config {
    query: String,
    file: String,
}

impl Config {
    fn new(args: &[String]) -> Result<Config, &'static str> {
        return parse_args(args);
    }
}

fn parse_args(args: &[String]) -> Result<Config, &'static str> {
    if args.len() != 2 {
        eprintln!("Arguments not equal to 2!");
        // process::exit(1);
        return Err("Arguments not equal to 2!");
    }

    let query = args[0].clone();
    let file = args[1].clone();
    Ok(Config {query, file})
}

fn grep_file(config: Config) -> () {
    println!("Search for: {}\n", config.query);
    let contents = fs::read_to_string(config.file)
        .expect("Should have been able to read the file");
    println!("With text:\n{contents}");
}
