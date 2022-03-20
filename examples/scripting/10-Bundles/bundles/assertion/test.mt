// Returns a function that throws an error when a test statement is not true.

return ::(condition) {
    when (condition == false) error(detail:'Assertion failure!'); 
};
