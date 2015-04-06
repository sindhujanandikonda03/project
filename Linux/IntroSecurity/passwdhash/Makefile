
all: passwd-plain login-plain passwd-hashed login-hashed passwd-salted login-salted passwd-pbkdf2 login-pbkdf2

%: %.c
	gcc -o $@ $^ -lcrypto
  
