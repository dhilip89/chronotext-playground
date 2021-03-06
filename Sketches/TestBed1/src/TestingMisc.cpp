/*
 * THE CHRONOTEXT-PLAYGROUND: https://github.com/arielm/chronotext-playground
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/chronotext-playground/blob/master/LICENSE
 */

#include "TestingMisc.h"

#include "chronotext/Context.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/incubator/utils/FileCapture.h"

#include <regex>

using namespace std;
using namespace ci;
using namespace chr;

void TestingMisc::run(bool force)
{
    if (force || true)
    {
        CHR_TEST(force || true, testSharedPtrCasting);
    }

    if (force || true)
    {
        CHR_TEST(force || true, testFileCapture);
        CHR_TEST(force || true, testNewLogging);
        CHR_TEST(force || true, testNewException);
        CHR_TEST(force || true, testInputSourceRobustness);
        CHR_TEST(force || true, testTimeFormat);
        CHR_TEST(force || true, testDurationFormat);
        
        CHR_TEST(force || true, testReadTextFile);
        CHR_TEST(force || true, testReadXmlFile);
        CHR_TEST(force || true, testReadU16StringToString);
        CHR_TEST(force || true, testReadStringToU16String);
    }
    
    if (force || true)
    {
        CHR_TEST(force || true, testStringToIntToString);
        CHR_TEST(force || true, testFileSystem);
    }
    
    if (force || true)
    {
        CHR_TEST(force || true, testRVOAndCopyElision);
        CHR_TEST(force || true, testMapInsertion);
    }
    
    if (force || true)
    {
        CHR_TEST(force || true, testRawLiterals1);
        CHR_TEST(force || true, testRawLiterals2);
    }
}

// ---

class ResourceItem
{
public:
    ResourceItem(shared_ptr<void> resource)
    :
    resource(resource)
    {}
    
    template <class T>
    inline std::shared_ptr<T> getResource()
    {
        if (this && resource)
        {
            return std::static_pointer_cast<T>(resource);
        }
        
        return nullptr;
    }
    
protected:
    shared_ptr<void> resource;
};

void TestingMisc::testSharedPtrCasting()
{
    TextureManager::LOG_VERBOSE = true;
    
    {
        TextureManager textureManager;
        
        auto texture = textureManager.getTexture(InputSource::getAsset("U_512.png"));
        CHR_CHECK(texture.use_count() == 2);
        
        auto resource = make_shared<ResourceItem>(texture);
        CHR_CHECK(texture.use_count() == 3);
        
        CHR_CHECK(resource->getResource<Texture>()->getSize() == Vec2i(512, 512));
        
        resource.reset();
        CHR_CHECK(texture.use_count() == 2);
    }
    
    TextureManager::LOG_VERBOSE = false;
}

// ---

void TestingMisc::testFileCapture()
{
    /*
     * TODO: ENHANCED API, E.G.
     *
     * FileCapture capture(stdout).setTrimTrailing(true);
     * printf("foo\n");
     * if (capture == "foo") cerr << capture << endl;
     */
    
    string outCapture;
    {
        FileCapture capture(stdout);
        printf("hello from stdout");
        outCapture = capture.flushAsString();
    }
    CHR_CHECK(outCapture == "hello from stdout");

    string errCapture;
    {
        FileCapture capture(stderr);
        fprintf(stderr, "hello from stderr\n");
        errCapture = capture.flushAsString(true);
    }
    CHR_CHECK(errCapture == "hello from stderr");
}

void TestingMisc::testNewLogging()
{
    /*
     * TODO: DEMONSTRATE "THREAD-BUFFERING" CAPABILITIES
     */
    
    LOGI << "thread-buffered and iomanip-aware" << ' ' << std::hex << 255 << std::dec << endl;
}

void TestingMisc::testNewException()
{
    try
    {
        InputSource::loadResource("undefined.png");
        CHR_CHECK(false);
    }
    catch (chr::Exception<InputSource> &e)
    {
        LOGI << e << endl; // I.E. SOMETHING WENT-WRONG WHILE LOADING
        return;
    }
    catch (exception &e)
    {
        LOGI << e.what() << endl; // E.G. SOMETHING WENT-WRONG WHILE DECODING THE IMAGE
    }
    
    CHR_CHECK(false);
}

void TestingMisc::testInputSourceRobustness()
{
    InputSource::Ref nonInitialized;

    /*
     * WITHOUT SOME "EXTRA-CARE" IN THE IMPLEMENTATION, THE FOLLOWING WOULD (BADLY) CRASH:
     */

    /*
     * INSTEAD: IT "ONLY" RETURNS A "DEFAULT-VALUE" (E.G. false)
     */
    nonInitialized->isFile();
    nonInitialized->getFilePath();
    nonInitialized->getFilePathHint();
    nonInitialized->setFilePathHint("bar");
    nonInitialized->getURI();

    /*
     * INSTEAD: IT "ONLY" THROWS
     */
    
    try
    {
        nonInitialized->loadDataSource();
        CHR_CHECK(false);
    }
    catch (chr::Exception<InputSource> &e)
    {
        LOGI << e << endl;
    }
    
    try
    {
        nonInitialized->getSubSource("foo");
        CHR_CHECK(false);
    }
    catch (chr::Exception<InputSource> &e)
    {
        LOGI << e << endl;
    }
}

void TestingMisc::testTimeFormat()
{
    CHR_CHECK(utils::format::time(255) == "04:15");
    CHR_CHECK(utils::format::time(4943) == "1:22:23");
    CHR_CHECK(utils::format::time(60) == "01:00");
    CHR_CHECK(utils::format::time(0.6, true) == "00:01");
}

void TestingMisc::testDurationFormat()
{
    CHR_CHECK(utils::format::duration(1.23456) == "1.2s");
    CHR_CHECK(utils::format::duration(0.0123456) == "12.3ms");
    CHR_CHECK(utils::format::duration(0.000123456) == "123.5μs");
    CHR_CHECK(utils::format::duration(0.00000123456) == "1.2μs");
    CHR_CHECK(utils::format::duration(0.0000000123456) == "12.3ns");
    CHR_CHECK(utils::format::duration(0.000000000123456) == "0.1ns");
    CHR_CHECK(utils::format::duration(60) == "01:00");
}

// ---

/*
 * TODO:
 *
 * 1) TEST ON iOS
 * 2) TEST ON ANDROID (PRE-REQUISITE FOR THE FOLLOWING 2: CREATING A "TRUE" FILE FROM THE ASSET)
 */

void TestingMisc::testReadTextFile()
{
    auto source = InputSource::getAsset("unicode.xml");
    
    if (source->isFile())
    {
        string text = utils::readTextFile(source->getFilePath());
        CHR_CHECK(std::hash<string>()(text) == SIZE_T2(4122924119, 7638217582490704265));
    }
}

void TestingMisc::testReadXmlFile()
{
    auto source = InputSource::getAsset("unicode.xml");
    
    if (source->isFile())
    {
        XmlTree xml = utils::readXmlFile(source->getFilePath());
        
        stringstream ss;
        ss << xml;
        
        CHR_CHECK(std::hash<string>()(ss.str()) == SIZE_T2(3254342745, 9170973041083656176));
    }
}

//

void TestingMisc::testReadU16StringToString()
{
    auto source = InputSource::getAsset("unicode.xml");
    
    auto textU16 = utils::readText<u16string>(source);
    auto text = utils::to<string>(textU16);
    
    CHR_CHECK(std::hash<string>()(text) == SIZE_T2(4122924119, 7638217582490704265));
}

void TestingMisc::testReadStringToU16String()
{
    auto source = InputSource::getAsset("unicode.xml");
    
    auto text = utils::readText<string>(source);
    auto textU16 = utils::to<u16string>(text);
    
    CHR_CHECK(std::hash<u16string>()(textU16) == SIZE_T2(2650149663, 14816650974461727307));
}

/*
 * TODO: TEST utils::writeTextFile() AND utils::writeXmlFile()
 */

// ---

/*
 * DEMONSTRATED:
 *
 * 1) "ROBUST" STRING-TO-INT CONVERSION:
 *    - ci::fromString<int>("2b") WOULD THROW (RELIES ON boost::lexical_cast BEHINDS THE SCENES)
 *    - std::stoi() WOULD BE THE PREFERRED (C++11) SOLUTION, BUT:
 *      - IT'S NOT SUPPORTED ON ANDROID (DETAILS FOLLOWING)
 *      - ALTERNATIVE: THE "OLD" std::atoi(), WHICH TAKES A const char* INSTEAD OF A std::string
 *
 * 2) INT-TO-STRING CONVERSION:
 *    - std::to_string() WOULD BE THE PREFERRED (C++11) SOLUTION, BUT:
 *      - IT'S NOT SUPPORTED ON ANDROID (DETAILS FOLLOWING)
 *      - ALTERNATIVE: ci::toString(), RELIES ON boost::lexical_cast BEHINDS THE SCENES
 *
 *
 * WHY STANDARD C++11 FEATURES LIKE stoi() AND to_string() ARE NOT SUPPORTED (AS OF NDK 10c) ON ANDROID?
 *
 * 1) DETAILS: http://stackoverflow.com/questions/17950814/how-to-use-stdstoul-and-stdstoull-in-android/18124627#18124627
 *
 * 2) SOLUTION TRIED: USING CRYSTAX NDK 10
 *    - https://www.crystax.net/en/android/ndk
 *    - stoi() AND to_string() DO WORK AS INTENDED, BUT:
 *      - boost::filesystem::path IS BROKEN, DUE TO std::use_facet, ETC.
 *        - DETAILS: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51813#c9
 *        - RECOMPILING BOOST FOR ANDROID COULD HELP, BUT IT'S A HELL:
 *          - TRIED: https://github.com/MysticTreeGames/Boost-for-Android
 *            - NO SUCCESS
 *            - IN ANY-CASE, THIS STUFF IS OUTDATED
 *              AND IT DOESN'T SEEM TO COMPILE EFFICIENT CODE:
 *              - ONLY HANDLING ARM-V5 ARCHITECTURE, ETC.
 *        - ALTERNATIVES:
 *          - TRYING TO USE CRYSTAX'S "BOOST FOR ANDROID" BUILD:
 *            - https://github.com/crystax/android-platform-ndk/blob/master/build/tools/build-boost.sh
 *            - CONS:
 *              - NO BINARIES CURRENTLY AVAILABLE:
 *                - BUILDING IS DEFINITELY NOT-TRIVIAL
 *              - BASED ON BOOST 1.57:
 *                - WE CURRENTLY USE 1.53
 *                - 1.55 IS KNOWN TO "WORK AS INTENDED" WITH CINDER
 *                - 1.57 REMAINS A MYSTERY
 *              - DEPENDS ON ICU-LIB:
 *                - IT IS SUPPOSED TO SOLVE ISSUES RELATED TO "LOCALE"
 *                - WE'RE NOT NECESSARILY INTERESTED AT THIS STAGE
 *                - AND WE ALREADY HAVE OUR VERSION OF ICU (USED BY THE ZFONT SYSTEM)
 *            - TODO:
 *              - WAIT FOR THE NEXT RELEASE OF CRYSTAX, IN THE HOPE FOR SOME BINARIES
 *
 */

void TestingMisc::testStringToIntToString()
{
    auto releaseString = "3.5.2b";
    auto components = ci::split(releaseString, '.'); // TODO: IMPLEMENT OUR OWN split (I.E. NOT RELYING ON CINDER NOR BOOST)

#if defined(CINDER_ANDROID)
    
    int major = (components.size() > 0) ? std::atoi(components[0].data()) : 0;
    int minor = (components.size() > 1) ? std::atoi(components[1].data()) : 0;
    int patch = (components.size() > 2) ? std::atoi(components[2].data()) : 0;
    
    CHR_CHECK(ci::toString(major) + "." + ci::toString(minor) + "." + ci::toString(patch) == "3.5.2"); // TODO: IMPLEMENT OUR OWN toString (I.E. NOT RELYING ON CINDER NOR BOOST)
    
#else
    
    int major = (components.size() > 0) ? std::stoi(components[0]) : 0;
    int minor = (components.size() > 1) ? std::stoi(components[1]) : 0;
    int patch = (components.size() > 2) ? std::stoi(components[2]) : 0;
    
    CHR_CHECK(std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch) == "3.5.2");
    
#endif
}

// ---

/*
 * ACCESSING HIERARCHICAL FILE-SYSTEM
 *
 * OSX: ANY PATH IS VALID
 * IOS: A COCOA "BUNDLE FOLDER" MUST BE USED (AND COPIED TO THE DEVICE VIA ITUNES-FILE-SHARING)
 * ANDROID: THE "EXTERNAL FOLDER" CAN BE USED
 */

void TestingMisc::testFileSystem()
{
    dumpDirectory(getPublicDirectory() / "test.bundle");
}

// ---

/*
 * TESTING RVO (RETURN VALUE OPTIMIZATION) AND COPY-ELISION: EVERYTHING WORKS AS INTENDED!
 *
 * REFERENCES:
 * - http://en.cppreference.com/w/cpp/language/copy_elision
 */

/*
 * RVO WILL TAKE PLACE (NO MATTER IF METHOD IS INLINE OR NOT)
 */
ObservableString TestingMisc::createWithRVO1()
{
    return ObservableString("bar"); // NO MATTER IF RETURNED VALUE IS "NOT NAMED"
}

/*
 * RVO WILL TAKE PLACE (NO MATTER IF METHOD IS INLINE OR NOT)
 */
ObservableString TestingMisc::createWithRVO2()
{
    ObservableString s("baz");
    return s;
}

/*
 * A TEMPORARY ObservableString IS CONSTRUCTED, BUT THANKS TO THE IMPLEMENTED MOVE-CONSTRUCTOR: INNER BYTES ARE NOT CLONED
 *
 * REMARKS:
 * - PROBABLY NOT FEASIBLE WITHOUT MOVE-CONSTRUCTION?
 * - WORKS NO MATTER IF METHOD IS INLINE OR NOT
 * - WILL LIKELY WORK WITH CLASSES LIKE std::string
 */
ObservableString&& TestingMisc::observeWhilePreservingRVO(ObservableString &&s)
{
    LOGI << __PRETTY_FUNCTION__ << endl;
    return move(s);
}

void TestingMisc::testRVOAndCopyElision()
{
    {
        /*
         * THANKS TO COPY-ELISION: NO NEED TO USE ObservableString s1("foo") IN ORDER TO AVOID TEMPORARIES
         */
        ObservableString s1 = "foo";
    }
    LOGI << endl;
    
    {
        /*
         * NO TEMPORARIES, THANKS TO RVO AND COPY ELISION
         */
        ObservableString s2 = createWithRVO1();
    }
    LOGI << endl;
    
    {
        ObservableString s3 = createWithRVO2();
    }
    LOGI << endl;
    
    {
        ObservableString s4 = "FOO";
        ObservableString s5 = s4; // NO TEMPORARIES, THANKS TO COPY ELISION
    }
    LOGI << endl;
    
    {
        /*
         * RVO + MOVE-CONSTRUCTION + COPY ELISION
         */
        ObservableString s5 = observeWhilePreservingRVO(createWithRVO2());
    }
}

// ---

/*
 * CHECKING "IN PLACE" VALUE INSERTION
 */

void TestingMisc::testMapInsertion()
{
    map<int, ObservableString> map1;
    
    /*
     * NO EXTRA COPY
     *
     * THIS SIMPLE SYNTAX WORKS BECAUSE ObservableString IS CONSTRUCTIBLE WITH A SINGLE ARGUMENT
     */
    map1.emplace(99, "bar");
    
    /*
     * OTHERWISE, A MORE COMPLEX SYNTAX MUST BE USED
     */
    map1.emplace(piecewise_construct, forward_as_tuple(33), forward_as_tuple("foo"));
}

// ---

string RAWSTRING1 = R"(FIRST LINE\nSAME LINE)";

string RAWSTRING2 = R"((RAW WITH "QUOTES" IN PARENTHESES))";

string RAWSTRING3 = R"(LINE 1
LINE 2
LINE 3)";

void TestingMisc::testRawLiterals1()
{
    CHR_CHECK(RAWSTRING1 == "FIRST LINE\\nSAME LINE");
    
    CHR_CHECK(RAWSTRING2 == "(RAW WITH \"QUOTES\" IN PARENTHESES)");
    
    CHR_CHECK(RAWSTRING3 == "LINE 1\nLINE 2\nLINE 3");
}

void TestingMisc::testRawLiterals2()
{
    /*
     * ADVANTAGE OF C++11 RAW STRING LITERALS: NO NEED USE ESCAPE \
     */
    regex rr(R"(((\+|-)?[[:digit:]]+)(\.(([[:digit:]]+)?))?((e|E)((\+|-)?)[[:digit:]]+)?)");

    CHR_CHECK(regex_match("123.456", rr));
}
