use mini_redis::{client, Result};

#[tokio::main]
async fn main() -> Result<()> {
    let mut redis_cli = client::connect("127.0.0.1:6379").await?;

    redis_cli.set("hello", "world".into()).await?;

    let result = redis_cli.get("hello").await?;
    println!("get result from redis server:{:?}", result);

    Ok(())
}