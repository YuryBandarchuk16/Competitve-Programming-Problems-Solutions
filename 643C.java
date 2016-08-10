//http://codeforces.com/contest/643/problem/C
import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.StringTokenizer;
import java.io.BufferedReader;
import java.io.InputStream;

/**
 * Built using CHelper plug-in
 * Actual solution is at the top
 */
public class 643C {
    public static void main(String[] args) {
        InputStream inputStream = System.in;
        OutputStream outputStream = System.out;
        InputReader in = new InputReader(inputStream);
        PrintWriter out = new PrintWriter(outputStream);
        TaskC solver = new TaskC();
        solver.solve(1, in, out);
        out.close();
    }

    static class TaskC {
        public void solve(int testNumber, InputReader in, PrintWriter out) {
            int n = in.nextInt(), k = in.nextInt();
            int[] t = in.readIntArray(n);
            double[] sum = new double[n];
            double[] invSum = new double[n];
            double[] prefSum = new double[n];
            for (int i = 0; i < n; i++) {
                sum[i] = 1.0 * t[i];
                invSum[i] = 1.0 / sum[i];
            }
            for (int i = 0; i < n - 1; i++) {
                sum[i + 1] += sum[i];
                invSum[i + 1] += invSum[i];
            }
            for (int i = 0; i < n; i++) {
                prefSum[i] = sum[i] / (1.0 * t[i]);
            }
            for (int i = 0; i < n - 1; i++) {
                prefSum[i + 1] += prefSum[i];
            }
            ConvexHull convexHull = new ConvexHull();
            double[][] f = new double[n + 2][2];
            convexHull.add(new Line(0.0, 0.0));
            for (int kk = 1; kk <= k; kk++) {
                for (int i = 0; i < n; i++) {
                    f[i][1] = prefSum[i] + convexHull.getValueAtX(-invSum[i]);
                }
                convexHull.size = 0;
                convexHull.lines.clear();
                for (int i = 0; i < n; i++) {
                    convexHull.add(new Line(sum[i], invSum[i] * sum[i] - prefSum[i] + f[i][1]));
                    f[i][0] = f[i][1];
                    f[i][1] = 0.0;
                }
            }
            out.printf(Locale.US, "%.10f\n", f[n - 1][0]);
        }

        class Line {
            double k;
            double b;

            Line(double k, double b) {
                this.k = k;
                this.b = b;
            }

            double getXIntersection(Line l) {
                return (this.b - l.b) / (l.k - this.k);
            }

            double getValue(double x) {
                return k * x + b;
            }

            boolean better(Line a, Line b) {
                double xWithA = getXIntersection(a);
                double xWithB = getXIntersection(b);
                if (xWithA > xWithB) {
                    return true;
                }
                return false;
            }

        }

        class ConvexHull {
            int size = 0;
            List<Line> lines;

            ConvexHull() {
                size = 0;
                lines = new ArrayList<>();
            }

            void add(Line l) {
                if (size < 2) {
                    if (size < lines.size()) {
                        lines.set(size, l);
                        ++size;
                        return;
                    }
                    ++size;
                    lines.add(l);
                    return;
                }
                while (size >= 2) {
                    Line toCheck = lines.get(size - 2);
                    Line withWhom = lines.get(size - 1);
                    if (toCheck.better(l, withWhom)) {
                        --size;
                        continue;
                    } else {
                        break;
                    }
                }
                if (size < lines.size()) {
                    lines.set(size, l);
                    ++size;
                } else {
                    ++size;
                    lines.add(l);
                }
            }

            double getValueAtX(double x) {
                int le = 0;
                int ri = size - 1;
                while (ri - le > 1) {
                    int mid = (le + ri) >> 1;
                    double firstValue = lines.get(mid).getValue(x);
                    double secondValue = lines.get(mid + 1).getValue(x);
                    if (firstValue < secondValue) {
                        ri = mid;
                    } else {
                        le = mid;
                    }
                }
                double result = lines.get(ri).getValue(x);
                result = Math.min(result, lines.get(le).getValue(x));
                if (le - 1 >= 0) {
                    result = Math.min(result, lines.get(le - 1).getValue(x));
                }
                return result;
            }

        }

    }

    static class InputReader {
        public BufferedReader reader;
        public StringTokenizer tokenizer;

        public InputReader(InputStream stream) {
            reader = new BufferedReader(new InputStreamReader(stream), 32768);
            tokenizer = null;
        }

        public String next() {
            while (tokenizer == null || !tokenizer.hasMoreTokens()) {
                try {
                    tokenizer = new StringTokenizer(reader.readLine());
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
            return tokenizer.nextToken();
        }

        public int[] readIntArray(int n) {
            int[] ret = new int[n];
            for (int i = 0; i < n; i++) {
                ret[i] = nextInt();
            }
            return ret;
        }

        public int nextInt() {
            return Integer.parseInt(next());
        }

    }
}
