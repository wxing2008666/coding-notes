pub mod brute_force;

pub trait Solution {
    fn convert(s: String, num_rows: i32) -> String;
}