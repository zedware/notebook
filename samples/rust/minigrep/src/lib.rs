use std::error::Error;
use std::fs;

pub struct Config {
    query: String,
    file: String,
}

impl Config {
    pub fn new(args: &[String]) -> Result<Config, &'static str> {
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
    Ok(Config { query, file })
}

pub fn grep_file(config: Config) -> Result<(), Box<dyn Error>> {
    println!("Search for: {}\n", config.query);
    let contents = fs::read_to_string(config.file)?;
    println!("With text:\n{contents}");
    Ok(())
}
