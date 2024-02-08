#include <fstream>
#include <iostream>
#include <map>

//#include <TreeNode.hpp>
#include <DecisionTree.hpp>
#include <boost/serialization/map.hpp>

namespace Serializer {

//
void serializeNode(const TreeNode &node, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << node;
}

//
TreeNode deserializeNode(const std::string &filename) {
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
void serializeTree(const DecisionTree &tree, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << tree;
}

//
DecisionTree deserializeTree(const std::string &filename) {
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
void serializeMap(const std::map<Key, Value> &inputMap,
                  const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << inputMap;
}

//
template <typename Key, typename Value>
std::map<Key, Value> deserializeMap(const std::string &filename) {
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
} // namespace Serializer