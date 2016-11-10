#include "ofxVectorAnimation.h"

static const int DEFAULT_FRAME_RATE = 30;

ofxVectorAnimation::Shape::Shape() : ofPath::ofPath()
{
}

ofxVectorAnimation::Shape::Shape(const Shape& o) : ofPath::ofPath()
{
    ofPath::append(o);
    ofPath::setFilled(o.isFilled());
    ofPath::setFillColor(o.getFillColor());
    ofPath::setStrokeColor(o.getStrokeColor());
    ofPath::setStrokeWidth(o.getStrokeWidth());
    ofPath::setCurveResolution(o.getCurveResolution());
    ofPath::setCircleResolution(o.getCircleResolution());
}

ofxVectorAnimation::Frame::Frame()
{
}

ofxVectorAnimation::Frame::Frame(const Frame& o)
{
    shapes.resize(o.shapes.size());
    for (const auto& e : o.shapes)
    {
        shapes.push_back(e);
    }
}


ofxVectorAnimation::ofxVectorAnimation()
: mCurrentNumFrame(0)
, bPlaying(false)
, bLoop(true)
, mTick(0)
, mFrameRate(DEFAULT_FRAME_RATE)
{
    addFrame(); // first frame
    mDrawArea.set(0, 0, ofGetWidth(), ofGetHeight());
    mTmpShape.setStrokeWidth(1);
    mTmpShape.setStrokeColor(ofColor::black);
    mTmpShape.setFillColor(ofColor::black);
    mTmpShape.setFilled(false);
    allocateDrawBuffer();
}

ofxVectorAnimation::~ofxVectorAnimation()
{
    
}

void ofxVectorAnimation::update()
{
    if (bPlaying)
    {
        mTick += ofGetLastFrameTime();
        if (mTick > (1.0 / mFrameRate))
        {
            nextFrame(true);
            mTick = 0;
        }
    }

}

void ofxVectorAnimation::draw()
{
    draw(mCurrentNumFrame);
}

void ofxVectorAnimation::draw(int numFrame)
{
    if (numFrame >= 0 || numFrame < mFrames.size())
    {
        mDrawBuffer.begin();
        ofClear(0);
        for (const auto& e : mFrames[numFrame]->shapes)
        {
            e.draw();
        }
        if (numFrame == mCurrentNumFrame)
        {
            mTmpShape.draw();
        }
        
        mDrawBuffer.end();
        mDrawBuffer.draw(0, 0);
    }
}

void ofxVectorAnimation::addVertex(int x, int y)
{
    mTmpShape.lineTo(x, y);
}

void ofxVectorAnimation::addVertex(const ofPoint &point)
{
    addVertex(point.x, point.y);
}

void ofxVectorAnimation::establishVertices()
{
    mFrames[mCurrentNumFrame]->shapes.push_back(mTmpShape);
    mTmpShape.clear();
}

int ofxVectorAnimation::addFrame()
{
    mFrames.push_back(shared_ptr<Frame>(new Frame()));
    mCurrentNumFrame = mFrames.size() - 1;
    return mFrames.size();
}

int ofxVectorAnimation::insertFrame(int frame)
{
    mFrames.insert(mFrames.begin() + frame, shared_ptr<Frame>(new Frame()));
    return mFrames.size();
}

void ofxVectorAnimation::removeFrame()
{
    if (mFrames.size() > 1)
    {
        mFrames.erase(mFrames.begin() + mCurrentNumFrame);
        mCurrentNumFrame--;
    }
}

void ofxVectorAnimation::clear()
{
    mFrames.clear();
    addFrame();
}

void ofxVectorAnimation::nextFrame(bool loop)
{
    mCurrentNumFrame++;
    if (mCurrentNumFrame >= mFrames.size())
    {
        if (loop)
            mCurrentNumFrame = 0;
        else
            mCurrentNumFrame = mFrames.size() - 1;
    }
}

void ofxVectorAnimation::backFrame()
{
    if (mCurrentNumFrame - 1 >= 0)
    {
        mCurrentNumFrame--;
    }
}

void ofxVectorAnimation::play()
{
    mTick = 0;
    bPlaying = true;
}

void ofxVectorAnimation::stop()
{
    bPlaying = false;
}

void ofxVectorAnimation::setLoop(bool loopEnable)
{
    bLoop = loopEnable;
}

void ofxVectorAnimation::changeFrame(int frame)
{
    if (frame >= 0 && frame < mFrames.size())
    {
        mCurrentNumFrame = frame;
    }
}

void ofxVectorAnimation::togglePlay()
{
    bPlaying ? stop() : play();
}



static bool writePointToXml(const ofPoint& pt, const string& childName, ofXml *xml)
{
    bool success = true;
    success &= xml->addChild(childName);
    success &= xml->setTo(childName);
    success &= xml->addValue("X", pt.x);
    success &= xml->addValue("Y", pt.y);
    success &= xml->addValue("Z", pt.z);
    success &= xml->setToParent();
    return success;
}

static bool writeRectToXml(const ofRectangle& rect, const string& childName, ofXml *xml)
{
    bool success = true;
    success &= xml->addChild(childName);
    success &= xml->setTo(childName);
    success &= xml->addValue("X", rect.x);
    success &= xml->addValue("Y", rect.y);
    success &= xml->addValue("W", rect.width);
    success &= xml->addValue("H", rect.height);
    success &= xml->setToParent();
    return true;
}

static bool writeColorToXml(const ofColor& color, const string& childName, ofXml *xml)
{
    bool success = true;
    success &= xml->addChild(childName);
    success &= xml->setTo(childName);
    success &= xml->addValue<int>("R", (int)color.r);
    success &= xml->addValue<int>("G", (int)color.g);
    success &= xml->addValue<int>("B", (int)color.b);
    success &= xml->addValue<int>("A", (int)color.a);
    success &= xml->setToParent();
    return success;
}


template <typename T>
static T getValue_(ofXml& xml, const string& path, const string& errMsg) throw (const char*)
{
    T inf = std::numeric_limits<T>::min();
    T value = xml.getValue<T>(path, inf);
    if (value == inf) throw errMsg.c_str();
        return value;
}
#define getInt(xml,path) getValue_<int>(xml,path,"failed get int value")
#define getFloat(xml,path) getValue_<float>(xml,path,"failed get float value")


static ofPoint readPointFromXml(ofXml &xml, const string& childName) throw (const char*)
{
    const auto errValue = std::numeric_limits<float>::infinity();
    
    if (xml.exists(childName))
    {
        if (xml.setTo(childName))
        {
            auto x = xml.getValue<float>("X", errValue);
            auto y = xml.getValue<float>("Y", errValue);
            auto z = xml.getValue<float>("Z", errValue);
            if (x == errValue || y == errValue || z == errValue)
            {
                throw "got default value";
            }
            if (xml.setToParent())
            {
                return ofPoint(x, y, z);
            }
            else
                throw "failed set to parent";
        }
    }
    throw "can't set to child";
}

static ofRectangle readRectFromXml(ofXml &xml, const string& childName) throw (const char*)
{
    const auto errValue = std::numeric_limits<float>::infinity();
    
    if (xml.exists(childName))
    {
        if (xml.setTo(childName))
        {
            auto x = xml.getValue<float>("X", errValue);
            auto y = xml.getValue<float>("Y", errValue);
            auto w = xml.getValue<float>("W", errValue);
            auto h = xml.getValue<float>("H", errValue);
            if (x == errValue || y == errValue || w == errValue || h == errValue)
            {
                throw "got default value";
            }
            if (xml.setToParent())
            {
                return ofRectangle(x, y, w, h);
            }
            else
                throw "failed set to parent";
        }
    }
    throw "can't set to child";
}

static ofColor readColorFromXml(ofXml &xml, const string& childName) throw (const char*)
{
    const int errValue = std::numeric_limits<int>::min();
    
    if (xml.exists(childName))
    {
        if (xml.setTo(childName))
        {
            int r = xml.getValue<int>("R", errValue);
            int g = xml.getValue<int>("G", errValue);
            int b = xml.getValue<int>("B", errValue);
            int a = xml.getValue<int>("A", errValue);
            if (r == errValue || g == errValue || b == errValue || a == errValue)
            {
                throw "got default value";
            }
            if (xml.setToParent())
            {
                return ofColor(r, g, b, a);
            }
            throw "failed set to parent";
        }
    }
    throw "can't set to child";
}




bool ofxVectorAnimation::save(const string &filename)
{
    ofXml xml;
    
    try
    {
        xml.addChild("OFX_VECTOR_ANIMATION");
        xml.setTo("OFX_VECTOR_ANIMATION");
        
        // write general information
        xml.addChild("GENERAL_INFO");
        xml.setTo("GENERAL_INFO");
        writeRectToXml(mDrawArea, "DRAW_AREA", &xml);
        xml.setToParent(); //GENERAL_INFO
        
        
        for (int i = 0; i < mFrames.size(); ++i)
        {
            ofXml frameXml;
            
            // TODO: return bool check
            
            // write frame information
            frameXml.addChild("FRAME");
            frameXml.setTo("FRAME");
            frameXml.setAttribute("index", ofToString(i));
            
            // write shapes
            frameXml.addChild("SHAPE");
            frameXml.setTo("SHAPE");
            
            int j = 0;
            for (const auto& shape : mFrames[i]->shapes)
            {
                ofXml shapeXml;
                shapeXml.addChild("PATH");
                shapeXml.setTo("PATH");
                shapeXml.setAttribute("index", ofToString(j));
                
                shapeXml.addValue<int>("FILL", shape.isFilled());
                shapeXml.addValue<float>("STOROKE_WIDTH", shape.getStrokeWidth());
                writeColorToXml(shape.getFillColor(), "FILL_COLOR", &shapeXml);
                writeColorToXml(shape.getStrokeColor(), "STOROKE_COLOR", &shapeXml);
                shapeXml.addValue<int>("CURVE_RESOLUTION", shape.getCurveResolution());
                shapeXml.addValue<int>("CIRCLE_RESOLUTION", shape.getCircleResolution());
                
                // write path commands
                for (const auto& command : shape.getCommands())
                {
                    ofXml commandXml;
                    commandXml.addChild("COMMAND");
                    commandXml.setTo("COMMAND");
                    commandXml.addValue<int>("TYPE", static_cast<int>(command.type));
                    writePointToXml(command.to, "TO", &commandXml);
                    writePointToXml(command.cp1, "CP1", &commandXml);
                    writePointToXml(command.cp2, "CP2", &commandXml);
                    commandXml.addValue<float>("RADIUS_X", command.radiusX);
                    commandXml.addValue<float>("RADIUS_Y", command.radiusY);
                    commandXml.addValue<float>("ANGLE_BEGIN", command.angleBegin);
                    commandXml.addValue<float>("ANGLE_END", command.angleEnd);
                    shapeXml.addXml(commandXml);
                }
                frameXml.addXml(shapeXml);
                ++j;
            }
            frameXml.setToParent();
            xml.addXml(frameXml);
        }
        xml.setToParent(); //OFX_VECTOR_ANIMATION
    }
    catch (...)
    {
        ofLogError(__FUNCTION__, "Write XML Exception!");
        return false;
    }
    bool res = xml.save(filename);
    if (res == false) ofLogError(__FUNCTION__, "Failed save xml");
    return res;
}

bool ofxVectorAnimation::load(const string &filename)
{
    ofXml xml;
    if (xml.load(filename) == false)
    {
        ofLogError(__FUNCTION__, "Failed load file: " + filename);
        return false;
    }
    
    mFrames.clear();

    try
    {
        xml.setTo("OFX_VECTOR_ANIMATION");
        
        // read general information
        xml.setTo("GENERAL_INFO");
        mDrawArea = readRectFromXml(xml, "DRAW_AREA");
        xml.setToParent();
        
        // read frames
        if (!xml.setTo("FRAME[0]")) throw;
        
        do {
            auto frame = make_shared<Frame>();
            
            // read shape
            if (!xml.setTo("SHAPE")) throw;
            // read paths
                
            if (xml.getName() == "SHAPE" && xml.setTo("PATH[0]"))
            {
                do {
                    Shape shape;
                    
                    shape.setFilled(getInt(xml, "FILL") == 0 ? false : true);
                    shape.setStrokeWidth(getFloat(xml, "STOROKE_WIDTH"));
                    shape.setFillColor(readColorFromXml(xml, "FILL_COLOR"));
                    shape.setStrokeColor(readColorFromXml(xml, "STOROKE_COLOR"));
                    shape.setCurveResolution(getInt(xml, "CURVE_RESOLUTION"));
                    shape.setCircleResolution(getInt(xml, "CIRCLE_RESOLUTION"));
                    
                    // read commands
                    
                    if (xml.setTo("COMMAND[0]"))
                    {
                        do {
                            switch(getInt(xml, "TYPE"))
                            {
                                case ofPath::Command::moveTo:
                                    shape.moveTo(readPointFromXml(xml, "TO"));
                                    break;
                                case ofPath::Command::lineTo:
                                    shape.lineTo(readPointFromXml(xml, "TO"));
                                    break;
                                case ofPath::Command::curveTo:
                                    shape.curveTo(readPointFromXml(xml, "TO"));
                                    break;
                                case ofPath::Command::bezierTo:
                                    shape.bezierTo(readPointFromXml(xml, "CP1"), readPointFromXml(xml, "CP2"), readPointFromXml(xml, "TO"));
                                    break;
                                case ofPath::Command::quadBezierTo:
                                    shape.quadBezierTo(readPointFromXml(xml, "CP1"), readPointFromXml(xml, "CP2"), readPointFromXml(xml, "TO"));
                                    break;
                                case ofPath::Command::arc:
                                    shape.arc(readPointFromXml(xml, "TO"),
                                              getFloat(xml, "RADIUS_X"), getFloat(xml, "RADIUS_Y"),
                                              getFloat(xml, "ANGLE_BEGIN"), getFloat(xml, "ANGLE_END"));
                                    break;
                                case ofPath::Command::arcNegative:
                                    shape.arcNegative(readPointFromXml(xml, "TO"),
                                                      getFloat(xml, "RADIUS_X"), getFloat(xml, "RADIUS_Y"),
                                                      getFloat(xml, "ANGLE_BEGIN"), getFloat(xml, "ANGLE_END"));
                                    break;
                                case ofPath::Command::close:
                                    shape.close();
                                    break;
                            }
                        }
                        while (xml.setToSibling());
                        xml.setToParent();
                    }
                    frame->shapes.push_back(shape);
                }
                while (xml.setToSibling());
                xml.setToParent();
            }
            mFrames.push_back(frame);
            xml.setToParent();
        }
        while (xml.setToSibling());
        xml.setToParent();
    }
    catch (...)
    {
        ofLogError(__FUNCTION__, "Load xml exception!");
        return false;
    }
    if (mFrames.empty()) addFrame();
    return true;
}

bool ofxVectorAnimation::save()
{
    auto res = ofSystemSaveDialog("animation_sequence.dat", "Choose Save Path");
    if (res.bSuccess)
    {
        return save(res.getPath());
    }
    return false;
}

bool ofxVectorAnimation::load()
{
    auto res = ofSystemLoadDialog();
    if (res.bSuccess)
    {
        return load(res.getPath());
    }
    return false;
}

void ofxVectorAnimation::allocateDrawBuffer()
{
    mDrawBuffer.allocate(mDrawArea.width, mDrawArea.height, GL_RGBA);
}
