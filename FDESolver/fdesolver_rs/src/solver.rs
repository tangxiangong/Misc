#[warn(clippy::upper_case_acronyms)]
pub struct FDE {
    pub order: f64,
    pub inteval: f64,
    pub init: f64,
    pub spectrum: f64,
    pub source: fn(f64) -> f64,
}

pub struct Discretization {
    pub eq: FDE,
    pub step: f64,
}

fn diff<T>(v: &[T]) -> Vec<T>
where
    T: std::ops::Sub<Output = T> + Copy,
{
    if v.len() <= 1 {
        return Vec::new();
    }
    v.windows(2).map(|w| w[1] - w[0]).collect()
}

pub fn solve(pro: &Discretization) -> (Vec<f64>, Vec<f64>) {
    let (tau, eq) = (pro.step, &pro.eq);
    let (alpha, lambda) = (eq.order, eq.spectrum);
    let nums = (pro.eq.inteval / tau).floor() as usize;
    let b: Vec<_> = (0..nums)
        .map(|j| {
            let temp = (j as f64 + 1.0).powf(1.0 - alpha) - (j as f64).powf(1.0 - alpha);
            temp / (1.0 - alpha).gamma()
        })
        .collect();
    let c = diff(&b);
    let mut t = vec![0.0; nums + 1];
    let mut x = vec![0.0; nums + 1];
    x[0] = eq.init;
    let a = b[0] - tau.powf(alpha) * lambda;
    for n in 1..nums + 1 {
        t[n] = n as f64 * tau;
        let temp = c[0..n - 1]
            .iter()
            .zip(x[1..n].iter().rev())
            .map(|(&x, &y)| x * y)
            .reduce(|res, v| res + v)
            .unwrap_or(0.0);
        x[n] = (tau.powf(alpha) * (eq.source)(t[n]) + b[n - 1] * x[0] - temp) / a;
    }
    (t, x)
}
