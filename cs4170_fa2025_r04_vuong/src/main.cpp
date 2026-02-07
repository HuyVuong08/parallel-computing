#include <mpi.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/access.hpp>

class Particle{
    
    public:
        Particle() : Particle(0) {}

        Particle(int Nd){
            R.resize(Nd, -std::numeric_limits<double>::infinity());
            V.resize(Nd, -std::numeric_limits<double>::infinity());
            pBestPos.resize(Nd, -std::numeric_limits<double>::infinity());

            M = -std::numeric_limits<double>::infinity();
            pBest = -std::numeric_limits<double>::infinity();
        }

        void print(){
            std::cout << "M: " << M << "\n";
            std::cout << "pBest: " << pBest << "\n";

            for (int i = 0; i < (int)R.size(); i++){
                std::cout << "R[" << i << "] = " << R[i] << "\n";
            }
            for (int i = 0; i < (int)V.size(); i++){
                std::cout << "V[" << i << "] = " << V[i] << "\n";
            }
        }

        // pointer accessors return const pointers (prevent external mutation)
        const double* R_ptr() const { return R.empty() ? nullptr : R.data(); }
        const double* V_ptr() const { return V.empty() ? nullptr : V.data(); }
        const double* pBestPos_ptr() const { return pBestPos.empty() ? nullptr : pBestPos.data(); }
        const double* M_ptr() const { return &M; }
        const double* pBest_ptr() const { return &pBest; }

        double getM() const { return M; }
        double getPBest() const { return pBest; }
        double getR(int i) const { return R.at(i); }
        double getV(int i) const { return V.at(i); }
        double getPBestPos(int i) const { return pBestPos.at(i); }

        void setM(double v) { M = v; }
        void setPBest(double v) { pBest = v; }
        void setR(int i, double v) { R.at(i) = v; }
        void setV(int i, double v) { V.at(i) = v; }
        void setPBestPos(int i, double v) { pBestPos.at(i) = v; }
    
    private:
        std::vector<double> R;
        std::vector<double> V;
        double M;
        double pBest;
        std::vector<double> pBestPos;
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int) {
            ar & R & V & M & pBest & pBestPos;
        }
};


void MPI_Boost(){
    namespace mpi = boost::mpi;
    mpi::communicator world;

    int Nd = 3;
    Particle p(Nd);

    if (world.rank() == 0){
        for (int i = 0; i < Nd; ++i){
            p.setR(i, 1.0 + i);
            p.setV(i, 2.0 + i);
            p.setPBestPos(i, 3.0 + i);
        }
        p.setM(42.0);
        p.setPBest(7.0);
    }

    mpi::broadcast(world, p, 0);

    std::cout << "Rank " << world.rank() << " (Boost):\n";
    p.print();
}

void MPI_DerivedDataType(){
    int numProcs, myRank;
    constexpr int Nd = 3;

    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    Particle p(Nd);

    if (myRank == 0){
        for (int i = 0; i < Nd; ++i){
            p.setR(i, 1.0 + i);
            p.setV(i, 2.0 + i);
            p.setPBestPos(i, 3.0 + i);
        }
        p.setM(42.0);
        p.setPBest(7.0);
    }

    std::vector<double> buf;
    if (myRank == 0) {
        buf.reserve(3*Nd + 2);
        for (int i = 0; i < Nd; ++i) buf.push_back(p.getR(i));
        for (int i = 0; i < Nd; ++i) buf.push_back(p.getV(i));
        for (int i = 0; i < Nd; ++i) buf.push_back(p.getPBestPos(i));
        buf.push_back(p.getM());
        buf.push_back(p.getPBest());
    }

    int bufSize = static_cast<int>(buf.size());
    MPI_Bcast(&bufSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (myRank != 0) buf.resize(bufSize);
    MPI_Bcast(buf.data(), bufSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (myRank != 0) {
        int idx = 0;
        for (int i = 0; i < Nd; ++i) p.setR(i, buf[idx++]);
        for (int i = 0; i < Nd; ++i) p.setV(i, buf[idx++]);
        for (int i = 0; i < Nd; ++i) p.setPBestPos(i, buf[idx++]);
        p.setM(buf[idx++]);
        p.setPBest(buf[idx++]);
    }

    std::cout << "Rank " << myRank << ":\n";
    p.print();
};

int main(){
    MPI_Init(NULL, NULL);

    MPI_DerivedDataType();

    MPI_Boost();

    MPI_Finalize();
    return 0;
}