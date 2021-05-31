/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LIWECHATBACKTRACE_JIT_DEBUG_H
#define _LIWECHATBACKTRACE_JIT_DEBUG_H

#include <stdint.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <unwindstack/Global.h>
#include <unwindstack/Memory.h>

#include "BacktraceDefine.h"
#include "QuickenTable.h"
#include "DebugGlobal.h"

namespace unwindstack {
    enum ArchEnum : uint8_t;

    class Elf;
}

namespace wechat_backtrace {

    // Forward declarations.
    class Maps;

    class DebugJit : public DebugGlobal {
    public:
        explicit DebugJit(std::shared_ptr<unwindstack::Memory> &memory);

//        DebugJit(std::shared_ptr<Memory>& memory, std::vector<std::string>& search_libs);
        virtual ~DebugJit();

        unwindstack::Elf *GetElf(Maps *maps, uint64_t pc);

        bool GetFutSectionsInMemory(
                Maps *maps,
                uint64_t pc,
                /* out */ std::shared_ptr<wechat_backtrace::QutSections> &fut_sections);

        static std::shared_ptr<DebugJit> &Instance();

    private:
        void Init(Maps *maps);

        uint64_t (DebugJit::*read_descriptor_func_)(uint64_t) = nullptr;

        uint64_t (DebugJit::*read_entry_func_)(uint64_t *, uint64_t *) = nullptr;

        uint64_t ReadDescriptor32(uint64_t);

        uint64_t ReadDescriptor64(uint64_t);

        uint64_t ReadEntry32Pack(uint64_t *start, uint64_t *size);

        uint64_t ReadEntry32Pad(uint64_t *start, uint64_t *size);

        uint64_t ReadEntry64(uint64_t *start, uint64_t *size);

        bool ReadVariableData(uint64_t ptr_offset) override;

        void ProcessArch() override;

        uint64_t entry_addr_ = 0;
        bool initialized_ = false;
        std::vector<unwindstack::Elf *> elf_list_;      // TODO

        std::map<uint64_t, std::shared_ptr<wechat_backtrace::QutSectionsInMemory>> qut_in_memory_;

        std::mutex lock_;

        std::mutex lock_for_qut_;

        const bool log = false;
    };

}  // namespace unwindstack

#endif  // _LIWECHATBACKTRACE_JIT_DEBUG_H