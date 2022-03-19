use tokio::time::{self, Duration, Instant, sleep};

async fn task_that_takes_a_second() {
    println!("hello");
    time::sleep(time::Duration::from_secs(1)).await
}

#[tokio::main]
async fn main() {
    let mut interval = time::interval(time::Duration::from_secs(2));
    for _i in 0..5 {
        interval.tick().await;
        task_that_takes_a_second().await;
    }

    //
    let now = Instant::now();
    sleep(Duration::new(1, 0)).await;
    let new_now = Instant::now();
    println!("{:?}", new_now.checked_duration_since(now));
    println!("{:?}", now.checked_duration_since(new_now)); // None
}
