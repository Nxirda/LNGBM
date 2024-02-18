#include "Serializer.hpp"

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
void serialize_Answers(const Answers &ans, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << ans;
}

//
Answers deserialize_Answers(const std::string &filename) {
  std::ifstream ifs(filename, std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: Unable to open file '" << filename << "'" << std::endl;
    return Answers();
  }

  boost::archive::binary_iarchive ia(ifs);

  Answers ans;
  ia >> ans;

  return ans;
}
} // namespace Serializer