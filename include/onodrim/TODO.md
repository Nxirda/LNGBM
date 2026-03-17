# Goals

## Operators
[] - Operators should you CRTP and be an extension point, they also probably have a missleading name  
[] - Enum operators should probably not exist, use a concept instead and do not make it be bounded to implemented typed

## Criterias
[] - Criterias should you CRTP and be an extension point, they also probably have a missleading name  
[] - Enum criterias should probably not exist, use a concept instead and do not make it be bounded to implemented typed  
[] - Code could probably be mutualized and only require a simple object.

## Types
[] - Implement dataset as a proper type with minimal data storage  
[] - Parsing the data and filling the dataset should be separated   
[] - dataset, tree, forest, histogram should be specified in term of optimal memory representation  
[] - Use simpler types than unordered maps in critical sections.  
[] - Bin type shall be simpler.  
[] - Model itself might be fine as a pure virtual base class.  
[] - Training elements are not in the correct place  

## Code
[] - Lot of switch case on strings to select model, criteria. This is should be simplified.  
[] - Remove boost dependancy  
[] - Refactor serializer and timer 