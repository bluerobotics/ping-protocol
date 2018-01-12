#include "link.h"
#include "protocol.h"

class Ping : public QObject
{
    Q_OBJECT
public:
    Ping();
    ~Ping();

private:
    Link* _link;
    Protocol* _protocol;
};
