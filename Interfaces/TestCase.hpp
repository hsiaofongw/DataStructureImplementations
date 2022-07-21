//
// Created by 韦晓枫 on 2022/7/21.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_TESTCASE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_TESTCASE_HPP

namespace Interface {
    template <typename T1, typename T2>
    struct TestCase {
        T1 question;
        T2 expectedOutput;
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_TESTCASE_HPP
