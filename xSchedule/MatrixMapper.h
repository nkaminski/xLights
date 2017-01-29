#ifndef MATRIXMAPPER_H
#define MATRIXMAPPER_H

#include <string>

class wxXmlNode;

typedef enum {HORIZONTAL, VERTICAL} MMORIENTATION;
typedef enum {BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT} MMSTARTLOCATION;

class MatrixMapper 
{
    int _changeCount;
    int _lastSavedChangeCount;
    std::string _name;
	int _strings;
	int _strandsPerString;
	int _stringLength;
	MMORIENTATION _orientation;
	MMSTARTLOCATION _startLocation;
    size_t _startChannel;

public:

		static MMORIENTATION EncodeOrientation(const std::string orientation);
		static std::string DecodeOrientation(MMORIENTATION orientation);
		static MMSTARTLOCATION EncodeStartLocation(const std::string startLocation);
		static std::string DecodeStartLocation(MMSTARTLOCATION startLocation);

        MatrixMapper(wxXmlNode*n);
        MatrixMapper(int strings, int strandsPerString, int stringLength, MMORIENTATION orientation, MMSTARTLOCATION startLocation, size_t startChannel, const std::string& name);
        MatrixMapper(int strings, int strandsPerString, int stringLength, const std::string& orientation, const std::string& startLocation, size_t startChannel, const std::string& name);
        MatrixMapper();
        virtual ~MatrixMapper() {}
		size_t Map(int x, int y) const;
		int GetChannels() const;
		int GetWidth() const;
		int GetHeight() const;
        size_t GetStartChannel() const { return _startChannel; }
        std::string GetName() const { return _name; }
        int GetStrings() const { return _strings; }
        int GetStringLength() const { return _stringLength; }
        int GetStrandsPerString() const { return _strandsPerString; }
        std::string GetStartLocation() const { return DecodeStartLocation(_startLocation); }
        std::string GetOrientation() const { return DecodeOrientation(_orientation); }
        bool IsDirty() const { return _lastSavedChangeCount != _changeCount; }
        void ClearDirty() { _lastSavedChangeCount = _changeCount; }
        wxXmlNode* Save();
        void SetName(const std::string& name) { if (name != _name) { _name = name; _changeCount++; } }
        void SetStrings(const int strings) { if (strings != _strings) { _strings = strings; _changeCount++; } }
        void SetStringLength(const int stringLength) { if (stringLength != _stringLength) { _stringLength = stringLength; _changeCount++; } }
        void SetStrandsPerString(const int strandsPerString) { if (strandsPerString != _strandsPerString) { _strandsPerString = strandsPerString; _changeCount++; } }
        static void Test();
};

#endif
