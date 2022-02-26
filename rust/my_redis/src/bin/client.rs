use bytes::Bytes;
use mini_redis::client;
use tokio::sync::{mpsc, oneshot};

#[derive(Debug)]
enum Command {
    Get {
        key: String,
        resp: Responser<Option<Bytes>>,
    },
    Set {
        key: String,
        val: Bytes,
        resp: Responser<()>,
    },
}

type Responser<T> = oneshot::Sender<mini_redis::Result<T>>;

#[tokio::main]
async fn main() {
    let (tx, mut rx) = mpsc::channel(32);
    let tx2 = tx.clone();

    let manager = tokio::spawn(async move{
        let mut client = client::connect("127.0.0.1:6379").await.unwrap();

        while let Some(cmd) = rx.recv().await {
            match cmd {
                Command::Get{key, resp} => {
                    let res = client.get(&key).await;
                    let _ = resp.send(res);
                },
                Command::Set{key, val, resp} => {
                    let res = client.set(&key, val).await;
                    let _ = resp.send(res);
                }
            }
        }
    });

    let task1 = tokio::spawn(async move{
        let (resp_tx, resp_rx) = oneshot::channel();

        let cmd = Command::Get {
            key: "foo".to_string(),
            resp: resp_tx,
        };

        if tx.send(cmd).await.is_err() {
            println!("connection task1 shutdown");
            return;
        }

        let res = resp_rx.await;
        println!("Got (get) result from server:{:?}", res);
    });

    let task2 = tokio::spawn(async move{
        let (resp_tx, resp_rx) = oneshot::channel();

        let cmd = Command::Set {
            key: "foo".to_string(),
            val: "bar".into(),
            resp: resp_tx,
        };

        if tx2.send(cmd).await.is_err() {
            println!("connection task2 shutdown");
            return;
        }

        let res = resp_rx.await;
        println!("Got (set) result form server:{:?}", res);
    });

    task1.await.unwrap();
    task2.await.unwrap();
    manager.await.unwrap();
}