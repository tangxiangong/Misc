pub fn select_sort<T: PartialOrd + Copy, const N: usize>(arr: &mut [T; N]) {
    if N==0 {
        return;
    }
    for i in 0..N-1 {
        let mut mininum = arr[i];
        let mut min_index = i;
        for (j, v) in arr.iter().enumerate().skip(i) {
            if *v <= mininum {
                mininum = *v;
                min_index = j;
            }
        }
        arr[min_index] = arr[i];
        arr[i] = mininum;
    }
}

