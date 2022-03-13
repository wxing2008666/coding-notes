pub fn heap_sort<T: PartialOrd>(arr: &mut [T]) {
    let size = arr.len();
    for i in (0..size / 2).rev() {
        build_max_heap(arr, i, size - 1);
    }

    for i in (1..size).rev() {
        arr.swap(0, i);
        build_max_heap(arr, 0, i - 1);
    }
}

fn build_max_heap<T: PartialOrd>(arr: &mut [T], cur: usize, nums: usize) {
    let mut c = cur;
    let mut left_child = 2 * c + 1;
    loop {
        if left_child > nums {
            break;
        }

        if left_child < nums && arr[left_child] < arr[left_child + 1] {
            left_child += 1;
        }

        if arr[c] > arr[left_child] {
            break;
        } else {
            arr.swap(c, left_child);
        }

        c = left_child;
        left_child = left_child * 2 + 1;
    }
}

pub fn heap_sort_v2<T: PartialOrd>(arr: &mut [T]) {
    let size = arr.len();
    // 构建大根堆
    for i in (0..size / 2).rev() {
        heapify(arr, i, size);
    }

    // 每轮循环将堆顶元素（也就是最大元素）放到最后
    for i in (1..size).rev() {
        arr.swap(0, i);
        // 恢复大根堆
        heapify(arr, 0, i);
    }
}

fn heapify<T: PartialOrd>(arr: &mut [T], root: usize, end: usize) {
    // 记录父节点和左右节点中最大元素的索引位置
    let mut largest = root;

    let left_child = 2 * root + 1;
    if left_child < end && arr[left_child] > arr[largest] {
        largest = left_child;
    }

    let right_child = left_child + 1;
    if right_child < end && arr[right_child] > arr[largest] {
        largest = right_child;
    }

    if largest != root {
        arr.swap(root, largest);
        heapify(arr, largest, end);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_empty_vec() {
        let mut empty_vec: Vec<String> = vec![];
        heap_sort(&mut empty_vec);
        assert_eq!(empty_vec, Vec::<String>::new());
    }

    #[test]
    fn test_number_vec() {
        let mut vec = vec![7, 49, 73, 58, 30, 72, 44, 78, 23, 9];
        heap_sort(&mut vec);
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
        heap_sort(&mut vec);
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

    #[test]
    fn test_number_vec_v2() {
        let mut vec = vec![7, 49, 73, 58, 30, 72, 44, 78, 23, 9];
        heap_sort_v2(&mut vec);
        assert_eq!(vec, vec![7, 9, 23, 30, 44, 49, 58, 72, 73, 78]);
    }

    #[test]
    fn test_string_vec_v2() {
        let mut vec = vec![
            String::from("Bob"),
            String::from("David"),
            String::from("Carol"),
            String::from("Alice"),
        ];
        heap_sort_v2(&mut vec);
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