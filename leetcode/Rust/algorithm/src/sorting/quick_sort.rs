pub fn quick_sort<T: PartialOrd>(arr: &mut [T]) {
    if arr.len() > 1 {
        quick_sort_helper(arr, 0, arr.len() - 1);
    }
}

fn quick_sort_helper<T: PartialOrd>(arr: &mut [T], l: usize, h: usize) {
    if l < h {
        let tmp = l;
        let (mut i, mut j) = (l, h);
        while i < j {
            while i < j && arr[j] >= arr[tmp] {
                j -= 1;
            }

            while i < j && arr[i] <= arr[tmp] {
                i += 1;
            }
            if i != j {
                arr.swap(j, i);
            }
        }
        arr.swap(i, tmp);
        quick_sort_helper(arr, l, i - 1);
        quick_sort_helper(arr, i + 1, h);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_empty_vec() {
        let mut empty_vec: Vec<String> = vec![];
        quick_sort(&mut empty_vec);
        assert_eq!(empty_vec, Vec::<String>::new());
    }

    #[test]
    fn test_number_vec() {
        let mut vec = vec![7, 49, 73, 58, 30, 72, 44, 78, 23, 9];
        quick_sort(&mut vec);
        assert_eq!(vec, vec![7, 9, 23, 30, 44, 49, 58, 72, 73, 78]);
    }

    #[test]
    fn test_string_vec() {
        let mut vec = vec![
            String::from("Bob"),
            String::from("David"),
            String::from("Carol"),
            String::from("Alice"),
        ];
        quick_sort(&mut vec);
        assert_eq!(
            vec,
            vec![
                String::from("Alice"),
                String::from("Bob"),
                String::from("Carol"),
                String::from("David"),
            ]
        );
    }
}