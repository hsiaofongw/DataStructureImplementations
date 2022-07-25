//
// Created by 韦晓枫 on 2022/7/25.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MEETINGSCHEDULER_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MEETINGSCHEDULER_HPP


#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace SystemDesign::MeetingScheduler {
    class Scheduler {
    public:
        Scheduler() : slots() {
            this->slots.resize(8);
            for (size_t slotId = 0; slotId < 8; ++slotId) {
                this->slots[slotId] = 0;
            }
        }

        void printSlots() {
            for (size_t i = 0; i < 8; ++i) {
                std::cout << slots[i] << "  ";
            }
            std::cout << "\n";
        }

        /**
         *
         * @param duration 0 ... 7
         * @return nullptr if failed, meeting id if success.
         */
        std::shared_ptr<uint8_t> scheduleMeeting(uint8_t duration) {
            std::vector<uint8_t> roomSize (8, 0);
            roomSize[7] = slots[7] == 1 ? 0 : 1;
            for (size_t dj = 1; dj <= 7; ++dj) {
                size_t j = 7-dj;
                roomSize[j] = 0;
                if (slots[j] == 0) {
                    roomSize[j] = roomSize[j+1] + 1;
                }
            }

//            for (size_t i = 0; i < 8; ++i) {
//                for (size_t j = i; j < 8 && slots[j] == 0; ++j) {
//                    roomSize[i] = roomSize[i] + 1;
//                }
//            }

            for (size_t roomId = 0; roomId < 8; ++roomId) {
                if (roomSize[roomId] >= duration) {
                    for (size_t i = 0; i < duration; ++i) {
                        this->slots[roomId+i] = 1; // occupied
                    }
                    this->meetingLength[roomId] = duration;
                    return std::make_shared<uint8_t>(roomId);
                }
            }

            return nullptr;
        }

        void cancel(uint8_t meetingId) {
            uint8_t length = this->meetingLength[meetingId];
            this->meetingLength.erase(meetingId);
            for (size_t i = 0; i < length; ++i) {
                // free slot
                this->slots[meetingId+i] = 0;
            }
        }
    private:
        std::vector<uint8_t> slots;
        std::unordered_map<uint8_t, uint8_t> meetingLength;
    };
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_MEETINGSCHEDULER_HPP
