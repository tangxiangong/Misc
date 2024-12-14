use std::{error::Error, fmt::Display};

#[derive(Debug)]
pub enum MathError {
    DividedByZero,
    SquaredRoootOfZero,
}

impl Display for MathError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self {
            MathError::DividedByZero => write!(f, "Divided by Zero!"),
            MathError::SquaredRoootOfZero => write!(f, "Squared Root of Zero!"),
        }
    }
}

impl Error for MathError {}

pub fn sqrt(val: f64) -> Result<f64, MathError> {
    if val < 0.0 {
        Err(MathError::SquaredRoootOfZero)
    } else {
        Ok(val.sqrt())
    }
}

pub fn div(a: f64, b: f64) -> Result<f64, MathError> {
    if b == 0.0 {
        Err(MathError::DividedByZero)
    } else {
        Ok(a / b)
    }
}