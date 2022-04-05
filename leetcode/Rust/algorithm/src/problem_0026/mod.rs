pub mod remove_duplicates;

pub trait Solution {
    fn remove_duplicates(nums: Vec<i32>) -> i32;
}

#[cfg(test)]
mod tests {
    use super::Solution;

    pub fn run<S: Solution>() {
        let test_cases = [];
    }
}