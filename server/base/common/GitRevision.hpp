/*
 * Created by Praying on 2018/2/10.
 * Copyright (c) Flow. All rights reserved.
 */


#pragma once

namespace Flow
{

    namespace GitRevision
    {
        const char *getHash();

        const char *getDate();

        const char *getBranch();

        const char *getCMakeCommand();

        const char *getBuildDirectory();

        const char *getSourceDirectory();

        const char *getMySQLExecutable();

        const char *getFullDatabase();

        const char *getHotfixesDatabase();

        const char *getFullVersion();

        const char *getCompanyNameStr();

        const char *getLegalCopyrightStr();

        const char *getFileVersionStr();

        const char *getProductVersionStr();
    }

}


