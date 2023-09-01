# Starting a listener on each server
def test(net):
    net.get("h1").sendCmd("python3 -m http.server 9000 &")
    net.get("h2").sendCmd("python3 -m http.server 9001 &")
    net.get("h3").sendCmd("python3 -m http.server 9002 &")
    net.get("h4").sendCmd("python3 -m http.server 9003 &")
    net.get("h5").sendCmd("python3 -m http.server 9004 &")
    net.get("h6").sendCmd("python3 -m http.server 9005 &")
    return