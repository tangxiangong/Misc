use std::{hash::Hash, vec};

#[derive(Debug, Clone)]
struct Pair<K: Hash + Clone + Eq, V: Clone + PartialEq> {
    key: K,
    value: V,
}

impl<K, V> Pair<K, V>
where
    K: Hash + Clone + Eq,
    V: Clone + PartialEq,
{
    fn new(key: K, value: V) -> Self {
        Pair { key, value }
    }
}

#[derive(Debug)]
pub struct HashMap<K, V>
where
    K: Hash + Clone + Eq,
    V: Clone + PartialEq,
{
    size: usize,
    capacity: usize,
    load_size: usize,
    load_thres: f64,
    extend_ratio: usize,
    buckets: Vec<Option<Vec<Pair<K, V>>>>,
}

fn calc_hash(_key: &impl Hash) -> usize {
    todo!()
}

impl<K, V> HashMap<K, V>
where
    K: Hash + Clone + Eq,
    V: Clone + PartialEq,
{
    pub fn new(capacity: usize) -> Self {
        HashMap {
            capacity,
            buckets: vec![None; capacity],
            ..Self::default()
        }
    }

    pub fn size(&self) -> usize {
        self.size
    }

    fn update_capacity(&mut self) {
        self.capacity *= self.extend_ratio
    }

    fn load_factor(&self) -> f64 {
        self.load_size as f64 / self.capacity as f64
    }

    fn is_thres_warning(&self) -> bool {
        self.load_factor() > self.load_thres
    }

    fn extend(&mut self) {
        self.update_capacity();
        let new_buckets: Vec<Option<Vec<Pair<K, V>>>> = vec![None; self.capacity];
        let old_buckets = std::mem::replace(&mut self.buckets, new_buckets);
        for vector in old_buckets {
            if vector.is_some() {
                for pair in vector.unwrap() {
                    self.add(pair.key, pair.value)
                }
            }
        }
    }

    fn calc_idx(&self, key: &K) -> usize {
        calc_hash(key) % self.capacity
    }

    pub fn add(&mut self, key: K, value: V) {
        if self.is_thres_warning() {
            self.extend();
        }
        let idx = self.calc_idx(&key);
        let src = &mut self.buckets[idx];
        if let Some(v) = src.as_mut() {
            for iterm in v.iter_mut() {
                if iterm.key == key {
                    if iterm.value != value {
                        iterm.value = value;
                        self.size += 1;
                        return;
                    } else {
                        return;
                    }
                }
            }
            v.push(Pair::new(key, value));
            self.size += 1;
        } else {
            src.replace(vec![Pair::new(key, value)]);
            self.size += 1;
            self.load_size += 1;
        }
    }
}

impl<K, V> Default for HashMap<K, V>
where
    K: Hash + Clone + Eq,
    V: Clone + PartialEq,
{
    fn default() -> Self {
        HashMap {
            size: 0,
            capacity: 0,
            load_size: 0,
            load_thres: 0.75,
            extend_ratio: 2,
            buckets: Vec::new(),
        }
    }
}
