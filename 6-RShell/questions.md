1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines when a command's output is fully received from the server by looking for the ASCII code 0x04 (RDSH_EOF_CHAR) to indicate the end of the response stream.
To handle partial reads and ensure complete message transmission, a technique is to use a loop to call recv() repeatedly until the end-of-stream marker is detected. Another technique is to see if the last byte of a chunk is the EOF character and replace it with a null terminator.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol should define message boundaries by using special delimiter characters such as a null byte. A challenge that can arise is not handled correctly is when the client sends "cat" but the receiver might only get "ca" and "t" in two seperate `revc' calls.

3. Describe the general differences between stateful and stateless protocols.

Unlike stateless protocols, stateful protocols maintain information about the client's session across multiple requests because in stateless protocols, the server doesn't store information about client sessions between requests because each request is independent.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is still useded to send message to multiple recipients such as broadcasting.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides a socket interface/abstraction such as socket API functions like bind(), listen(), send(), recv(), close().