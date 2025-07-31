# Report for Branch Predictor Accuracy

This is the report for branch predictor accuracy. 

In the form below, ATP means the predictor that always takes the jump instruction.
AFP means the predictor that always does not take the jump instruction.
PMP is the predictor based on perceptron model.

By the data given in the form below, it is easy to find under most testcases, PMP showcases a higher branch predictor accuracy.
Since the perceptron model actually needs a huge amount of data for training, the accuracy may be low when there are few jump instructions when executing the code.

| Testcase       | ATP    | AFP    | PMP    | Testcase       | ATP    | AFP    | PMP    |
|----------------|--------|--------|--------|----------------|--------|--------|--------|
| array_test1    | 0.4545 | 0.5455 | 0.5000 | array_test2    | 0.5000 | 0.5000 | 0.5769 |
| basicopt1      | 0.4102 | 0.5898 | 0.6948 | bulgarian      | 0.4936 | 0.5064 | 0.9436 |
| expr           | 0.6216 | 0.3884 | 0.8378 | gcd            | 0.3750 | 0.6250 | 0.5833 |
| hanoi          | 0.5002 | 0.4998 | 0.7355 | lvalue2        | 0.3333 | 0.6667 | 0.3333 |
| magic          | 0.4686 | 0.5314 | 0.7689 | manyarguments  | 0.2000 | 0.8000 | 0.2000 |
| multiarray     | 0.8148 | 0.1852 | 0.8272 | pi             | 0.5773 | 0.4227 | 0.8210 |
| qsort          | 0.6999 | 0.3001 | 0.8392 | queens         | 0.3678 | 0.6322 | 0.6912 |
| statement_test | 0.4059 | 0.5941 | 0.5545 | superloop      | 0.1271 | 0.8729 | 0.9225 |
| tak            | 0.2500 | 0.7500 | 0.7609 |                |        |        |        |