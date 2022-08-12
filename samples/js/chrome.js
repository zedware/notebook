function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

// Verify if sleep works or not.
async function sleep_and_log() {
    for (let i = 0; i < 2; i++) {
        console.log(`Waiting ${i} seconds...`);
        await sleep(i * 1000);
    }
    console.log('Done');
}

// Save DOM elements as global variable: temp5
async function go(n) {
    for (let i = 0; i < n; i++) {
        temp5.children[2].children[1].click();
        await sleep_and_log();
        console.log(temp5.children[0].textContent);
        console.log(temp5.children[3].textContent)
    }
}

// For example, click the "next" button 100 times, 
// wait for each click, then log the content of the page.
go(100);

