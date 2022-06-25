//
// Created by 韦晓枫 on 2022/6/25.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_ADDSTRINGFORMBINARY_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_ADDSTRINGFORMBINARY_HPP

#include <string>

namespace Algorithm {
    namespace AddStringFormBinary {
        using std::string;
        class Solution {
        public:
            string addBinary(string a, string b) {
                // carry_in, 1_in, 2_in, 1_out,
                uint8_t bitOut[2][2][2];
                bitOut[0][0][0] = 0;
                bitOut[0][1][0] = 1;
                bitOut[0][0][1] = 1;
                bitOut[0][1][1] = 0;
                bitOut[1][0][0] = 1;
                bitOut[1][1][0] = 0;
                bitOut[1][0][1] = 0;
                bitOut[1][1][1] = 1;

                // carry_in, 1_in, 2_in, carry_out
                uint8_t carryOut[2][2][2];
                carryOut[0][0][0] = 0;
                carryOut[0][1][0] = 0;
                carryOut[0][0][1] = 0;
                carryOut[0][1][1] = 1;
                carryOut[1][0][0] = 0;
                carryOut[1][1][0] = 1;
                carryOut[1][0][1] = 1;
                carryOut[1][1][1] = 1;

                uint8_t carryBit = 0;
                size_t minWidth = std::min(a.size(), b.size());
                size_t maxWidth = std::max(a.size(), b.size());
                string longer = "";
                string shorter = "";
                if (a.size() > b.size()) {
                    longer = a;
                    shorter = b;
                } else {
                    longer = b;
                    shorter = a;
                }
                for (size_t i = 0; i < minWidth; ++i) {
                    auto longerOffset = maxWidth-i-1;
                    auto shorterOffset = minWidth-i-1;
                    uint8_t in_1 = longer[longerOffset] == '1' ? 1 : 0;
                    uint8_t in_2 = shorter[shorterOffset] == '1' ? 1 : 0;
                    longer[longerOffset] = bitOut[carryBit][in_1][in_2] == 1 ? '1' : '0';
                    carryBit = carryOut[carryBit][in_1][in_2];
                }

                if (carryBit) {
                    for (size_t i = 0; i < (maxWidth - minWidth); ++i) {
                        auto offset = maxWidth - (minWidth+i)-1;
                        auto val = longer[offset] == '1' ? 1 : 0;
                        longer[offset] = bitOut[carryBit][val][0] == 1 ? '1' : '0';
                        carryBit = carryOut[carryBit][val][0];
                    }
                }

                if (carryBit) {
                    longer = '1' + longer;
                }

                return longer;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_ADDSTRINGFORMBINARY_HPP
