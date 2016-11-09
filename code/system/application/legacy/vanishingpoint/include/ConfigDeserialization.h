#ifndef CONFIG_DESERIALIZATION_H_
#define CONFIG_DESERIALIZATION_H_

namespace lmvp {

template<class T>
class BuilderParser {
public:
    virtual ~BuilderParser() = default;
    virtual operator T() const = 0;
    virtual void operator>>(std::istream &) = 0;

protected:
    template<class E>
    static void consumeKeyword(std::istream & stream, const std::string & keyword) {
        std::string keywordBuffer(keyword);

        stream.read(&keywordBuffer[0], keyword.length());

        if(0 != keywordBuffer.compare(keyword)) {
            throw E();
        }
    }

    template<class E>
    static cv::Point parsePoint(std::istream & stream) {
        cv::Point point;

        consumeKeyword<E>(stream, "(");
        stream >> point.x;
        consumeKeyword<E>(stream, ",");
        stream >> point.y;
        consumeKeyword<E>(stream, ")");

        return point;
    }
};

class RoiBuilder : public BuilderParser<RegionOfInterestGeometry> {
public:
    RoiBuilder() : topLeft(), bottomRight() {}

    operator RegionOfInterestGeometry() const {
        return RegionOfInterestGeometry(cv::Rect(topLeft, bottomRight));
    }

    void operator>>(std::istream & stream) {
        // FIXME: does not throw when no bottomRight is supplied
        consumeKeyword<Malformed>(stream, "topLeft");
        topLeft = parsePoint<Malformed>(stream);
        consumeKeyword<Malformed>(stream, ", ");

        consumeKeyword<Malformed>(stream, "bottomRight");
        bottomRight = parsePoint<Malformed>(stream);
    }

    class Malformed : std::runtime_error {
    public:
        Malformed() : std::runtime_error("Malformed regoin of interest string. Valid example:"
                                         "\"topLeft(108,330), bottomRight(595,460)\"") {}
    };
private:
    cv::Point topLeft, bottomRight;
};

class ScanRegionBuilder : public BuilderParser<ScanRegion> {
public:
    /**
     * only exists due to a bug in GCCs -Weffc++
     * http://stackoverflow.com/a/14002558
     * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=16166#c3
     */
    ScanRegionBuilder() : bottomLeft(), topLeft(), topRight(), bottomRight() {}
    
    operator ScanRegion() const {
        if(!valid) {
            throw Invalid();
        }

        return ScanRegion(bottomLeft, topLeft, topRight, bottomRight);
    }

    void operator>>(std::istream & stream) {
        consumeKeyword<Malformed>(stream, "bottomLeft");
        bottomLeft = parsePoint<Malformed>(stream);
        consumeKeyword<Malformed>(stream, ", ");

        consumeKeyword<Malformed>(stream, "topLeft");
        topLeft = parsePoint<Malformed>(stream);
        consumeKeyword<Malformed>(stream, ", ");

        consumeKeyword<Malformed>(stream, "topRight");
        topRight = parsePoint<Malformed>(stream);
        consumeKeyword<Malformed>(stream, ", ");

        consumeKeyword<Malformed>(stream, "bottomRight");
        bottomRight = parsePoint<Malformed>(stream);

        valid = true;
    }

    class Invalid : std::runtime_error {
    public:
        Invalid() : std::runtime_error("Tried to get a ScanRegion instance from the "
                                       "ScanRegionBuilder before supplying data.") {}
    };

    class Malformed : std::runtime_error {
    public:
        Malformed() : std::runtime_error("Malformed scan region string. Valid example: \""
                                         "bottomLeft(467,456), topLeft(393,325), "
                                         "topRight(482,326), bottomRight(608,420)\"") {}
    };

private:
    bool valid = false;
    cv::Point bottomLeft, topLeft, topRight, bottomRight;
};

template<class T>
std::istream & operator>>(std::istream & stream, BuilderParser<T> & builder) {
    builder.operator>>(stream);

    return stream;
}

}

#endif
