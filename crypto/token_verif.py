import jwt
from jwt.exceptions import InvalidSignatureError

public_key = """-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAw/L46s1SNhniqoBjg0BI
GuyQ0XpG/jYT5KZuAV+x97IGl3YBA2tVv2qkWTOLJoshzoaPuZ+A/GVfD/uNTy+s
QemY2wdxzXBiuFs56KHk4LT60O6InBU5baiqfO5cT9In53r7N44eIw7CLloCR4jr
31AiuQm1e8hm9xP8Vg3dVjHpgbzFwbXc8m187EXTKKOjVdIhNNf4+/0bEb9H6RVi
G7NTLt4AXzqs3gj57WYFQPwWHwtgd9EmenoiA1WEQXLtbT7CVqwiiQCRonoVamDa
tq8QXUerHYABx/Q9cjFVFR9e4XG/LHbPYUMdP+FuqTmoOHkApHLk5p/4p+bvwc0q
twIDAQAB
-----END PUBLIC KEY-----"""

token = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJBZG1pbiIsImV4cCI6MzYwMCwiaWF0IjowfQ.LRF9wVlNuWgkkzfsjT4Brz0znCMQUgoKLY_M2nKVF8qQvDH8PQha-7IqneckWYQGAFa-980jKDIJeiTsMvQcgxMPrJZ60liadYQl1uOaDv1QKAQvNuHkQOEr3SrNSf8zE4lsJ6rcFkxtiKgoKd0ppfrxp9WMsfg4SDrxxrH3L8TXyxTqs_wC-9yenqDqwicBlfOrrcEHYuDqQhJMznzf8dzsaCEVbKjV6hYuaEOYX7vL1UNm0iKpHhnhK9ygnoP75KYwxb_gmzr7N6vZ8II8qHNrEmDQ8LkIUpkAGc6wi23h61ZhGrzqcZTSTCOcgvPmwt21Rj0SW93mwXKd0jxbVg"

try:
    decoded = jwt.decode(token, public_key, algorithms=["RS256"], options={"verify_exp": False, "verify_aud": False})
    print("Signature validée. Contenu décodé :", decoded)
except InvalidSignatureError:
    print("Signature invalide ! Le JWT a été altéré ou la clé est incorrecte.")
except jwt.InvalidTokenError:
    print("Erreur de token JWT.")
