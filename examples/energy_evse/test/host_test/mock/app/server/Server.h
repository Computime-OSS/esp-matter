#pragma once

#include "credentials/FabricTable.h"
#include "lib/core/CHIPPersistentStorageDelegate.h"

namespace chip {

class Server {
public:
    static Server & GetInstance()
    {
        static Server inst;
        return inst;
    }

    FabricTable & GetFabricTable() { return mFabricTable; }
    PersistentStorageDelegate & GetPersistentStorage() { return mPersistentStorage; }

    InMemoryPersistentStorageDelegate & GetInMemoryStorage() { return mPersistentStorage; }

private:
    Server() = default;

    FabricTable mFabricTable;
    InMemoryPersistentStorageDelegate mPersistentStorage;
};

} // namespace chip
