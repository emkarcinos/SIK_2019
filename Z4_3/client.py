from smtplib import SMTP

sender = "komar86311@gmail.com"

msg ="""From: From Person <komar86311@gmail.com>
To: To Person <komar86311@gmail.com>
Subject: SMTP e-mail test

This is a test e-mail message.
"""
smtpConnection = SMTP(host='localhost', port=1025)
smtpConnection.sendmail(sender, sender, msg)

