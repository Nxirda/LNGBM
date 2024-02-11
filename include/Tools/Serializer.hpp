#include <fstream>
#include <iostream>
#include <map>

//#include <TreeNode.hpp>
#include <DecisionTree.hpp>
#include <boost/serialization/map.hpp>

namespace Serializer {

//
void serialize_Node(const TreeNode &node, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << node;
}

//
TreeNode deserialize_Node(const std::string &filename) {
  std::ifstream ifs(filename, std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: Unable to open file '" << filename << "'" << std::endl;
    return TreeNode();
  }

  boost::archive::binary_iarchive ia(ifs);

  TreeNode node;
  ia >> node;

  return node;
}

//
void serialize_Tree(const DecisionTree &tree, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << tree;
}

//
DecisionTree deserialize_Tree(const std::string &filename) {
  std::ifstream ifs(filename, std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: Unable to open file '" << filename << "'" << std::endl;
    return DecisionTree();
  }

  boost::archive::binary_iarchive ia(ifs);

  DecisionTree tree;
  ia >> tree;

  return tree;
}

//
template <typename Key, typename Value>
void serialize_Map(const std::map<Key, Value> &inputMap,
                  const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << inputMap;
}

//
template <typename Key, typename Value>
std::map<Key, Value> deserialize_Map(const std::string &filename) {
  std::ifstream ifs(filename, std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: Unable to open file '" << filename << "'" << std::endl;
    return std::map<Key, Value>();
  }

  boost::archive::binary_iarchive ia(ifs);

  std::map<Key, Value> deserializedMap;
  ia >> deserializedMap;

  return deserializedMap;
}

// Serialize function to convert arguments to a string and then serialize them
template<typename... Args>
std::string serialize_Data(const Args&... args) {
    std::stringstream ss;
    (ss << ... << args); // Serialize each argument into the stringstream
    return ss.str();
}

} // namespace Serializer