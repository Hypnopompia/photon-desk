SELECT
 STRFTIME("%Y-%m-%d", created_at) AS day,
 COUNT(id),
 COUNT(id) / 60 AS hours,
 SUM(standing) AS readingCount,
 ((SUM(standing)*1.0) / (COUNT(id)*1.0) ) * 100 AS percentStanding
FROM desk GROUP BY day ORDER BY id;

SELECT AVG(percentStanding) FROM (
 SELECT
  STRFTIME("%Y-%m-%d", created_at) AS day,
  COUNT(id),
  COUNT(id) / 60 AS hours,
  SUM(standing) AS readingCount,
  ((SUM(standing)*1.0) / (COUNT(id)*1.0) ) * 100 AS percentStanding
 FROM desk WHERE (STRFTIME("%Y", created_at) = '2017') GROUP BY day ORDER BY id) AS t;

SELECT
STRFTIME("%Y-%m-%d", created_at) AS day,
((SUM(standing)*1.0) / (COUNT(id)*1.0) ) * 100 AS percentStanding
FROM desk WHERE (atdesk=1) GROUP BY day ORDER BY id;