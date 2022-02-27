use tokio::sync::oneshot;

#[tokio::main]
async fn main() {
    let (tx1, rx1) = oneshot::channel();
    let (tx2, rx2) = oneshot::channel();

    tokio::spawn(async {
        let _ = tx1.send("test1");
    });

    tokio::spawn(async {
        let _ = tx2.send("test2");
    });

    tokio::select! {
        val1 = rx1 => {
            println!("tx1 done:{:?}", val1);
        },
        val2 = rx2 => {
            println!("tx2 done:{:?}", val2);
        },
    }
}
