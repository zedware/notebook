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
    println!("Search for: \n{}\n", config.query);
    let contents = fs::read_to_string(config.file).unwrap();
    println!("With text:\n{contents}");
    println!("Found:");
    let mut i = 0;
    for line in grep_string(&config.query, &contents) {
        println!("{line}");
        i = i + 1;
    }
    println!("\n{} lines found.", i);
    Ok(())
}

fn grep_string<'a>(query: &str, s: &'a str) -> Vec<&'a str> {
    let mut results = Vec::new();

    for line in s.lines() {
        if line.contains(query) {
            results.push(line);
        }
    }

    results
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn one_result() {
        let query = "swift";
        let contents = "\
taylor swift is \
a famous singer.";
        assert_eq!(vec!["taylor swift is a famous singer."], grep_string(query, contents));
    }

    #[test]
    fn two_results() {
        let query = "rust";
        let contents = "\
rust for dust.\n\
rust for steel.\n\
bye.";
        assert_eq!(vec!["rust for dust.", "rust for steel."], grep_string(query, contents));
    }

    #[test]
    fn rust_the_best() {
        let query = "rust";
        let contents = "rust is better than everyone else.";
        assert_eq!(vec!["rust is better than everyone else."], grep_string(query, contents));
    }
}
