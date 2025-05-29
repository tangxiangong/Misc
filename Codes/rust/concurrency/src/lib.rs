use std::sync::mpsc;
use std::thread;
use std::time::Duration;

pub fn threads() {
    let v = vec![1, 2, 3];
    thread::scope(|s| {
        s.spawn(|| {
            for i in 1..10 {
                println!("Hello number {i} from spawned thread 1");
                thread::sleep(Duration::from_millis(1));
            }
            println!("Spawned thread 1 is finished");
        });

        s.spawn(move || {
            for val in &v {
                println!("Value from spawned thread 2: {val}");
                thread::sleep(Duration::from_millis(1));
            }
            println!("Spawned thread 2 is finished");
        });

        s.spawn(|| {
            for i in 1..5 {
                println!("Hello number {i} from spawned thread 3");
                thread::sleep(Duration::from_millis(1));
            }
            println!("Spawned thread 3 is finished");
        });

        println!("Scope is finished");
    });

    for i in 1..5 {
        println!("Hello number {i} from main thread");
    }

    // println!("{:?}", v);
}

pub fn par_sum(arr: &[i32]) -> i32 {
    // 获取硬件的线程数
    let num_threads = thread::available_parallelism()
        .map(|p| p.get())
        .unwrap_or(1); // 如果无法获取，则默认使用1个线程

    println!("系统可用的并行线程数: {}", num_threads);
    // 创建一个通道，用于从工作线程发送数据到主线程
    let (tx, rx) = mpsc::channel();
    // 将数字分成几组，每个线程处理一部分
    let chunk_size = arr.len() / num_threads;
    let chunks = arr.chunks(chunk_size);

    // 为每个数据块创建一个线程
    for chunk in chunks {
        // 克隆发送者，这样每个线程都有自己的发送者
        let tx_clone = tx.clone();
        let chunk_owned = chunk.to_vec();

        thread::spawn(move || {
            // 计算当前块的和
            let sum = chunk_owned.iter().sum::<i32>();

            // 将结果发送回主线程
            tx_clone.send(sum).expect("无法发送结果");
        });
    }

    // 丢弃原始发送者，这样当所有克隆的发送者都超出范围时，接收者就会知道没有更多的消息了
    drop(tx);

    // 在主线程中接收并汇总结果
    let mut total = 0;
    for received in rx {
        total += received;
    }
    total
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_threads() {
        threads();
    }

    #[test]
    fn test_par_sum() {
        let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
        let sum = par_sum(&numbers);
        assert_eq!(sum, 55);
    }
}
