use core::panic;
use std::fmt::{Display, Debug};
use std::hash::{DefaultHasher, Hash, Hasher};
use std::ops::{Index, IndexMut};


#[derive(Debug, Clone)]
struct Pair<K: Hash + Clone + Eq + Debug, V: Clone + PartialEq + Debug> {
    key: K,
    value: V,
}

impl<K, V> Display for Pair<K, V> 
where 
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug, 
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?} => {:?}", self.key, self.value)
    }

}

impl<K, V> Pair<K, V> 
where
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug,
{
    fn new(key: K, value: V) -> Self {
        Pair { key, value }
    }
}

#[derive(Debug)]
pub struct HashMap<K, V> 
where
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug,
{
    size: usize,
    capacity: usize,
    load_size: usize,
    load_thres: f64,
    extend_ratio: usize,
    buckets: Vec<Option<Vec<Pair<K, V>>>>,
}

fn calc_hash(key: &impl Hash) -> u64 {
    let mut hasher = DefaultHasher::new();
    key.hash(&mut hasher);
    hasher.finish()    
}

impl<K, V> HashMap<K, V> 
where
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug,
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
        (self.load_size + 1) as f64 / self.capacity as f64
    }

    fn is_thres_warning(&self) -> bool {
        self.load_factor() > self.load_thres
    }

    fn extend(&mut self) {
        self.update_capacity();
        self.size = 0;
        self.load_size = 0;
        let new_buckets: Vec<Option<Vec<Pair<K, V>>>> = vec![None; self.capacity];
        let old_buckets = std::mem::replace(&mut self.buckets, new_buckets);
        for vector in old_buckets {
            if vector.is_some() {
                for pair in vector.unwrap() {
                    self.add(pair.key, pair.value);
                }
            }
        }
    }

    fn calc_idx(&self, key: &K) -> usize {
        calc_hash(key) as usize % self.capacity
    }

    pub fn add(&mut self, key: K, value: V) {
        if self.is_thres_warning() {
            self.extend();
        }
        let idx = self.calc_idx(&key);
        let src =  &mut self.buckets[idx];
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

    pub fn find(&self, key: &K) -> Option<&V> {
        let idx = self.calc_idx(key);
        let mut result: Option<&V> = None;
        if let Some(v) = self.buckets[idx].as_ref() {
            for iterm in v {
                if iterm.key == *key {
                    result = Some(& iterm.value);
                    break;
                }
             }
        }
        result
    }

    pub fn is_element(&self, key: &K, value: &V) -> bool {
        let val_op = self.find(key);
        if val_op.is_some() {
            let val: &V = val_op.unwrap();
            *val == *value
        } else {
            false
        }
    }
}

impl<K, V> Index<&K> for HashMap<K, V>  
where
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug,  
{
    type Output = V;
    /// 
    /// 返回 `key` 所对应值的引用
    /// 
    /// # Panic
    /// 
    /// 当 `idx` 不存在时 panic
    /// 
    fn index(&self, key: &K) -> &Self::Output {
        match self.find(key) {
            Some(value) => value,
            None => panic!("{:?} 不存在", key),
        }
    }     
}

impl<K, V> IndexMut<&K> for HashMap<K, V>  
where
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug,  
{
    /// 
    /// 返回 `key` 所对应值的引用
    /// 
    /// # Panic
    /// 
    /// 当 `idx` 不存在时 panic
    /// 
    fn index_mut(&mut self, key: &K) -> &mut Self::Output {
            let idx = self.calc_idx(key);
            let mut res = None;
            if let Some(v) = self.buckets[idx].as_mut() {
                for iterm in v.iter_mut() {
                    if iterm.key == *key {
                        res = Some(&mut iterm.value);
                    }
                 }
            }
            match res {
                Some(v) => v,
                None => panic!("{:?} 不存在！", key),
            }
        }
     
}

impl<K, V> Default for HashMap<K, V>  
where
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug,
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

impl<K, V> Display for HashMap<K, V> 
where 
    K: Hash + Clone + Eq + Debug,
    V: Clone + PartialEq + Debug, 
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut s =  format!("HashMap with size: {}, capacity: {}.\n", self.size, self.capacity);
        let mut temp = "\n".to_string();
        for iterm in self.buckets.iter() {
            if iterm.is_some() {
                let pair_vec = iterm.as_ref().unwrap();
                temp.push_str("    [ ");
                for pair in pair_vec.iter() {
                    temp.push_str(&format!("{}, ", pair));
                }
                temp.push_str("],\n");
            }
        }
        s.push_str(&format!("{{{}}} load factor: {}", temp, self.load_factor()));
        
        // write!(f, "load factor: {}", self.load_factor())
        f.write_str(&s)
    }

}
