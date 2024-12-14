#[derive(Debug)]
pub struct Pair<K, V> {
    key: K,
    value: V,
}

#[derive(Debug)]
pub struct HashMap<K, V> {
    size: usize,
    capacity: usize,
    load_thres: f64,
    extend_ratio: usize,
    buckets: Vec<Option<Vec<Pair<K, V>>>>
}