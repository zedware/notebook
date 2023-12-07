use std::env;
use std::process;

use minigrep::Config;

fn main() {
    let args: Vec<String> = env::args().skip(1).collect();
    dbg!(&args);
    let config = Config::new(&args).unwrap_or_else(|err| {
        println!("Problem parsing arguments: {err}");
        process::exit(1);
    });
    if let Err(e) = minigrep::grep_file(config) {
        println!("Application error: {e}");
        process::exit(1);
    }
}
