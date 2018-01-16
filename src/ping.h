#include "link.h"
#include "protocol.h"

class Ping : public QObject
{
    Q_OBJECT
public:
    Ping();
    ~Ping();

    Q_PROPERTY(Protocol* protocol READ protocol NOTIFY protocolUpdate)
    Protocol* protocol() { return _protocol; };

    Q_PROPERTY(Link* link READ link NOTIFY linkUpdate)
    Link* link() { return _link; };

private:
    Link* _link;
    Protocol* _protocol;

signals:
    void protocolUpdate();
    void linkUpdate();
};
