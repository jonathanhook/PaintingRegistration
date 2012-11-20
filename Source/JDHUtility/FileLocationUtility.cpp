/**
 * This file is part of PaintingRegistration.
 *
 * Created by Jonathan Hook (jonathan.hook@ncl.ac.uk)
 * Copyright (c) 2012 Jonathan Hook. All rights reserved.
 *
 * PaintingRegistration is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PaintingRegistration is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PaintingRegistration.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "FileLocationUtility.h"

namespace JDHUtility
{
    std::string FileLocationUtility::resourcePath = "";
    
    const std::string FileLocationUtility::getResourcePath(void)
    {
        return resourcePath;
    }
    
    const std::string FileLocationUtility::getFileInResourcePath(const std::string &path)
    {
        std::string p = resourcePath;
        p.append("/");
        p.append(path);
        return p;
    }
    
    void FileLocationUtility::setResourcePath(const std::string &path)
    {
        resourcePath = path;
    }
}