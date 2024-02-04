#include <fstream>
#include <iostream>

#include <TreeNode.hpp>

namespace Serializer {

void serializeNode(const TreeNode &node, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << node;
}

TreeNode deserializeNode(const std::string &filename) {
  std::ifstream ifs(filename, std::ios::binary);
  boost::archive::binary_iarchive ia(ifs);

  TreeNode node;
  ia >> node;

  return node;
}

} // namespace Serializer