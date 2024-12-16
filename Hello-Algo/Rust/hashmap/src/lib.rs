use std::fmt::{Debug, Display};
use std::hash::{DefaultHasher, Hash, Hasher};
use std::ops::{Index, IndexMut};

/// 处理未知 `key` 的情况
pub struct KeyError {
    key_str: String,
}

impl Debug for KeyError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "'{}' 不存在！", self.key_str)
    }
}

impl Display for KeyError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Error: '{}' 不存在！", self.key_str)
    }
}

impl std::error::Error for KeyError {}

/// 键值对类型
#[derive(Debug, Clone)]
pub(crate) struct Pair<K: Hash + Clone + Eq + Debug, V: Clone + PartialEq + Debug> {
    key: K,
    value: V,
}

impl<K, V> Display for Pair<K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?} => {:?}", self.key, self.value)
    }
}

impl<K, V> Pair<K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    fn new(key: K, value: V) -> Self {
        Pair { key, value }
    }
}

/// 哈希表
/// 
/// # Member
/// 
/// - `size`: 存储键值对的数量
/// - `capacity`: 容量
/// - `load_size`: 负载
/// - `load_thres`: 负载/容量比值上限, 默认为 0.75, 即超过这个比值时, 自动扩容
/// - `extend_ratio`: 扩容的倍数
/// - `buckets`: 存储键值对的容器
/// 
#[derive(Debug)]
pub struct HashMap<K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    size: usize,
    capacity: usize,
    load_size: usize,
    load_thres: f64,
    extend_ratio: usize,
    buckets: Vec<Option<Vec<Pair<K, V>>>>,
}

/// 计算 `key` 的哈希值
fn calc_hash(key: &impl Hash) -> u64 {
    let mut hasher = DefaultHasher::new();
    key.hash(&mut hasher);
    hasher.finish()
}

impl<K, V> HashMap<K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    /// 创建初始容量为 `capacity` 的哈希表
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

    pub fn is_empty(&self) -> bool {
        self.size == 0
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

    pub fn keys(&self) -> Vec<&K> {
        let mut result = Vec::<&K>::with_capacity(self.size);
        for (key, _) in self.iter() {
            result.push(key);
        }
        result
    }

    pub fn values(&self) -> Vec<&V> {
        let mut result = Vec::<&V>::with_capacity(self.size);
        for (_, value) in self.iter() {
            result.push(value);
        }
        result
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

    pub fn remove(&mut self, key: &K) -> Result<(), KeyError> {
        let idx = self.calc_idx(key);
        if let Some(v) = self.buckets[idx].as_mut() {
            let mut dest_idx = None;
            for (k, iterm) in v.iter().enumerate() {
                if iterm.key == *key {
                    dest_idx = Some(k);
                }
            }
            match dest_idx {
                Some(index) => {
                    v.swap_remove(index);
                    self.size -= 1;
                    if v.is_empty() {
                        self.buckets[idx] = None;
                        self.load_size -= 1;
                    }
                    Ok(())
                }
                None => Err(KeyError {
                    key_str: key.to_string(),
                }),
            }
        } else {
            Err(KeyError {
                key_str: key.to_string(),
            })
        }
    }

    pub fn find(&self, key: &K) -> Option<&V> {
        let idx = self.calc_idx(key);
        let mut result: Option<&V> = None;
        if let Some(v) = self.buckets[idx].as_ref() {
            for iterm in v {
                if iterm.key == *key {
                    result = Some(&iterm.value);
                    break;
                }
            }
        }
        result
    }

    pub fn find_mut(&mut self, key: &K) -> Option<&mut V> {
        let idx = self.calc_idx(key);
        let mut result = None;
        if let Some(v) = self.buckets[idx].as_mut() {
            for iterm in v.iter_mut() {
                if iterm.key == *key {
                    result = Some(&mut iterm.value);
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

    pub fn iter(&self) -> Iter<'_, K, V> {
        Iter::new(self)
    }
}

impl<K, V> Index<&K> for HashMap<K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
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
    K: Hash + Clone + Eq + Debug + Display,
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
        match self.find_mut(key) {
            Some(v) => v,
            None => panic!("{:?} 不存在！", key),
        }
    }
}

impl<K, V> Default for HashMap<K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
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
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut s = format!(
            "HashMap with size: {}, capacity: {}.\n",
            self.size, self.capacity
        );
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
        f.write_str(&s)
    }
}

pub struct Iter<'a, K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    data: &'a Vec<Option<Vec<Pair<K, V>>>>,
    size: usize,
    counter: usize,
    bucket_index: usize,
    pair_index: usize,
}

impl<'a, K, V> Iter<'a, K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    pub fn new(map: &'a HashMap<K, V>) -> Iter<'a, K, V> {
        Iter {
            data: &map.buckets,
            size: map.size,
            counter: 0,
            bucket_index: 0,
            pair_index: 0,
        }
    }
}

impl<'a, K, V> Iterator for Iter<'a, K, V>
where
    K: Hash + Clone + Eq + Debug + Display,
    V: Clone + PartialEq + Debug,
{
    type Item = (&'a K, &'a V);
    fn next(&mut self) -> Option<Self::Item> {
        if self.counter >= self.size {
            return None;
        }

        while self.bucket_index < self.data.len() {
            if let Some(pairs) = &self.data[self.bucket_index] {
                if self.pair_index < pairs.len() {
                    let pair = &pairs[self.pair_index];
                    self.pair_index += 1;
                    self.counter += 1;
                    return Some((&pair.key, &pair.value));
                }
                self.pair_index = 0;
            }
            self.bucket_index += 1;
        }
        None
    }
}
