#ifndef _VI_OBJECT_
#define _VI_OBJECT_





class Object
{
public:
    unsigned int id;
    unsigned int ref;
    unsigned int type;

public:
    Object();
    virtual ~Object();

    void retain();
    void release();
};

class ObjectMgr
{
public:
    static ObjectMgr* _inst;
    // id -> obj
    // id -> []exitfunc
    // id -> []loopfunc
    // [loop func]
    // [new ids]

public:
    static ObjectMgr* inst();

    unsigned id();
    void add(Object* obj);
    void del(Object* obj);

    void loop(float dt);
};




















#endif







