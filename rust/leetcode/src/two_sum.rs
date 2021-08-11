use std::collections::HashMap;
use std::collections::hash_map::Entry;

fn solution_optnums(nums: Vec<i32>, target: i32) -> Vec<i32> {
    let mut ans = vec![];
    let mut index_map = HashMap::<i32, Vec<usize>>::new();
    for (i, val) in nums.iter().enumerate() {
        match index_map.entry(*val) {
            Entry::Vacant(e) => { e.insert(vec![i]); },
            Entry::Occupied(mut e) => { e.get_mut().push(i); }
        }
    }

    for (i,val) in nums.iter().enumerate() {
        let new_target = target - val;
        if index_map.contains_key(&new_target) {
            if *val == new_target {
                if let Some(indices) = index_map.get(&new_target) {
                    if indices.len() < 2 {
                        continue;
                    }
                }
            }
            ans.push(i as i32);
            if let Some(indices) = index_map.get(&new_target) {
                ans.push(indices[0] as i32);
                return ans;
            }
        }
        ans.clear();
    }
    ans
}

fn solution(nums: Vec<i32>, target: i32) -> Vec<i32> {
    let len = nums.len();
    let mut ans: Vec<i32> = vec![];
    for i in 0..len {
        let new_target = target-nums[i];
        ans.push(i as i32);
        for j in i+1..len {
            if nums[j] == new_target {
                ans.push(j as i32);
                return ans;
            }
        }
        ans.pop();
    }
    ans
}
pub fn run()
{
    let ans = solution_optnums(vec![1,2,3,4,5], 7);
    println!("{:?}", ans);
}