import imaplib

IMAP_SERVER_IP = '192.168.14.24'

imap_client = imaplib.IMAP4(IMAP_SERVER_IP)
rc, resp = imap_client.login('bob', 'password')
print rc, resp

imap_client.select('Inbox')
r, data = imap_client.search(None, '(SUBJECT "FLAG")')

for num in data[0].split():
        typ, msg_itm = imap_client.fetch(num, '(RFC822)')
        print msg_itm

imap_client.logout()

