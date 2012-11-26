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
#include <string>

namespace JDHUtility
{
	class FileLocationUtility
	{
    public:
        static const std::string getDocumentsPath(void);
        static const std::string getResourcePath(void);
        static const std::string getFileInDocumentsPath(const std::string &path);
        static const std::string getFileInResourcePath(const std::string &path);
        static void setDocumentsPath(const std::string &path);
        static void setResourcePath(const std::string &path);
        
    private:
        static std::string documentsPath;
        static std::string resourcePath;
    };
}
