/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   broken_organ_xml;
    const int            broken_organ_xmlSize = 1580;

    extern const char*   default_xml;
    const int            default_xmlSize = 1580;

    extern const char*   distorted_trumpet_xml;
    const int            distorted_trumpet_xmlSize = 1580;

    extern const char*   honking_keychain_xml;
    const int            honking_keychain_xmlSize = 1580;

    extern const char*   off_to_space_xml;
    const int            off_to_space_xmlSize = 1651;

    extern const char*   shit_xml;
    const int            shit_xmlSize = 1643;

    extern const char*   wow_organ_xml;
    const int            wow_organ_xmlSize = 1579;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
