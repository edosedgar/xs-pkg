/* |==========================|
 * |Trie implemention on Swift|
 * |==========================|
 */

/*
 * One node of trie
 */
class trie_node_t<data_t:Hashable> {
        var data : data_t?
        var is_last : Bool
        var children : [data_t : trie_node_t]
        var used : Bool

        init(elem : data_t?) {
                data = elem
                is_last = false
                children = [data_t : trie_node_t]()
                used = false
        }
}

/*
 * General class
 */
class trie_t<data_t:Hashable> {
        let head : trie_node_t<data_t>
        /*
         * Init
         */
        init(data: [[data_t]]) {
                //Create main trie_node_t
                head = trie_node_t<data_t>(elem : nil)
                //Let's parse input data
                for first in data {
                        expand(data : first)
                        print("Added: \(first)")
                }
        }
        /*
         * Add new element
         */
        func expand(data: [data_t]) {
                var current_level : trie_node_t<data_t> = head
                var count = 0

                for letter in data {
                        if let next_stage = current_level.children[letter] {
                                current_level = next_stage
                                current_level.is_last = count == (data.count - 1)
                        } else {
                                current_level.children[letter] =
                                        trie_node_t<data_t>(elem : letter)
                                current_level.children[letter]!.is_last =
                                        count == (data.count - 1)
                                current_level = current_level.children[letter]!
                        }
                        count = count + 1
                }
        }
        /*
         * Search in trie
         */
        func prefix_search(data: [data_t]) -> [[data_t]]? {
                var current_level : trie_node_t<data_t> = head
                var common_node : trie_node_t<data_t>?
                var results : [[data_t]]? = [[data_t]]()

                //Let's find the deepest common node
                for letter in data {
                        if let next_stage = current_level.children[letter] {
                                current_level = next_stage
                        } else {
                                break
                        }
                        if letter == data.last {
                                common_node = current_level
                        }
                }
                //In case of absence of any matches
                guard let temp_node = common_node else {
                        return nil
                }
                //Let's build answers
                while let element = resolve_all(node: common_node,
                                                cur_data: data) {
                        results?.append(element)
                }
                clear_service_flag(node: head)
                return results
        }
        /*
         * Service function
         */
        private func resolve_all(node: trie_node_t<data_t>?,
                                 cur_data: [data_t]) -> [data_t]? {
                var loc_data : [data_t] = cur_data

                for (character, c_node) in node!.children {
                        loc_data = cur_data
                        if c_node.is_last && !c_node.used {
                                c_node.used = true
                                loc_data.append(character)
                                return loc_data
                        }
                        loc_data.append(character)
                        if let ret = resolve_all(node: c_node,
                                                 cur_data: loc_data) {
                                return ret
                        } else {
                                continue
                        }
                }

                if node!.is_last && !node!.used {
                        node!.used = true
                        return loc_data
                }
                return nil
        }
        /*
         * Service function
         */
        private func clear_service_flag(node: trie_node_t<data_t>) {
                for (_, c_node) in node.children {
                        c_node.used = false
                        clear_service_flag(node: c_node)
                }
                return
        }
}

/*
 * Main code
 */
var key = "te"
let instance = trie_t<Character>(data: [Array("three".characters),  Array("tea".characters),
                                        Array("tiny".characters), Array("telescope".characters),
                                        Array("bird".characters)])
var completion = instance.prefix_search(data: Array(key.characters))
print("\nKey: \"\(key)\"")
if let result = completion {
        for each in result {
                print("Found match:\(each)")
        }
} else {
        print("No result")
}

key = "t"
completion = instance.prefix_search(data: Array(key.characters))
print("\nKey: \"\(key)\"")
if let result = completion {
        for each in result {
                print("Found match:\(each)")
        }
} else {
        print("No result")
}
