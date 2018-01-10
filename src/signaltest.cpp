#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;

const double pi = 3.141592653589793;


//' Computes permutation test for Moran's I
//' 
//' @param xr a vector of data
//' @param Wr a weighting matrix
//' @param reps the number of permutations
// [[Rcpp::export]]
List moranTest(NumericVector xr, NumericMatrix Wr, unsigned int reps){
  
      double n = xr.size();
      arma::mat W(Wr.begin(), n, n, false);
      arma::colvec x(xr.begin(), n, false);
      
      double m = arma::mean(x);
      arma::colvec y = x - m;
      double ssm = arma::accu(W % (y * arma::trans(y)));
      double v = arma::accu(arma::pow(y, 2));
      double ns0 = n / arma::accu(W);
      double moran = ns0 * (ssm / v);
      
      
      arma::colvec sim(reps);
      arma::colvec xperm(n);
      for(unsigned int i = 0 ; i < reps ; i++){
        xperm = arma::shuffle(x);
        y = xperm - m;
        ssm = arma::accu(W % (y * arma::trans(y)));
        v = arma::accu(arma::pow(y, 2));
        sim(i) = ns0 * (ssm / v);
      }
      
      arma::uvec usup = arma::find(sim >= moran);
      double nsup = usup.n_elem;
      double pval = (nsup + 1) / (reps + 1);
      
      return Rcpp::List::create(Rcpp::Named("Moran.I") = moran,
                                Rcpp::Named("pvalue") = pval);
}



//' Computes permutation test for Blomberg's K
//' 
//' @param xr a vector of data
//' @param vcvr phylogenetic variance-covariance matrix
//' @param reps the number of permutations
// [[Rcpp::export]]
List kTest(NumericVector xr, NumericMatrix vcvr, unsigned int reps){
  
      double n = xr.size();
      arma::mat vcv(vcvr.begin(), n, n, false);
      arma::colvec x(xr.begin(), n, false);
      
      arma::colvec vcvDiag = vcv.diag();
      arma::mat vcvInv = arma::inv(vcv);
      double vcvInvAccu = arma::accu(vcvInv);
      double vcvDiagAccu = arma::accu(vcvDiag);
      
      double xhat = arma::accu(vcvInv * x) / vcvInvAccu;
      arma::colvec dx = x - xhat;
      arma::rowvec dxt = arma::trans(dx);
      
      double MSE0MSEobs = arma::as_scalar((dxt * dx) / (dxt * vcvInv * dx));
      double MSE0MSE = (1 / (n - 1)) * (vcvDiagAccu - (n / vcvInvAccu));
      double k = MSE0MSEobs/MSE0MSE;

      arma::colvec sim(reps);
      arma::colvec xperm(n);
      for(unsigned int i = 0 ; i < reps ; i++){
        xperm = arma::shuffle(x);
        xhat = arma::accu(vcvInv * xperm) / vcvInvAccu;
        dx = xperm - xhat;
        dxt = arma::trans(dx);
        MSE0MSEobs = arma::as_scalar((dxt * dx) / (dxt * vcvInv * dx));
        sim(i) = MSE0MSEobs/MSE0MSE;
      }
      
      arma::uvec usup = arma::find(sim >= k);
      double nsup = usup.n_elem;
      double pval = (nsup + 1) / (reps + 1);
      
      return Rcpp::List::create(Rcpp::Named("K") = k,
                                Rcpp::Named("pvalue") = pval);
}



//' Computes permutation test for Blomberg's K Star
//' 
//' @param xr a vector of data
//' @param vcvr phylogenetic variance-covariance matrix
//' @param reps the number of permutations
// [[Rcpp::export]]
List kStarTest(NumericVector xr, NumericMatrix vcvr, unsigned int reps){
  
      double n = xr.size();
      arma::mat vcv(vcvr.begin(), n, n, false);
      arma::colvec x(xr.begin(), n, false);
      
      arma::colvec vcvDiag = vcv.diag();
      arma::mat vcvInv = arma::inv(vcv);
      double vcvInvAccu = arma::accu(vcvInv);
      double vcvDiagAccu = arma::accu(vcvDiag);
      double vcvAccu = arma::accu(vcv);
      double m = arma::mean(x);
      
      double xhat = arma::accu(vcvInv * x) / vcvInvAccu;
      arma::colvec dx = x - xhat;
      arma::colvec dm = x - m;
      arma::rowvec dxt = arma::trans(dx);
      arma::rowvec dmt = arma::trans(dm);
      
      double MSE0MSEobs = arma::as_scalar((dmt * dm) / (dxt * vcvInv * dx));
      double MSE0MSE = (1 / (n - 1)) * (vcvDiagAccu - vcvAccu / n);
      double kstar = MSE0MSEobs/MSE0MSE;
        
      arma::colvec sim(reps);
      arma::colvec xperm(n);
      for(unsigned int i = 0 ; i < reps ; i++){
        xperm = arma::shuffle(x);
        xhat = arma::accu(vcvInv * xperm) / vcvInvAccu;
        dx = xperm - xhat;
        dm = xperm - m;
        dxt = arma::trans(dx);
        dmt = arma::trans(dm);
        MSE0MSEobs = arma::as_scalar((dmt * dm) / (dxt * vcvInv * dx));
        sim(i) = MSE0MSEobs/MSE0MSE;
      }
      
      arma::uvec usup = arma::find(sim >= kstar);
      double nsup = usup.n_elem;
      double pval = (nsup + 1) / (reps + 1);
      
      
      return Rcpp::List::create(Rcpp::Named("K.Star") = kstar,
                                Rcpp::Named("pvalue") = pval);
}



//' Computes log-likelihood for data and a given value of Pagel's Lambda
//' 
//' @param lambda the value of Pagel's Lambda
//' @param xr a vector of data
//' @param vcvr phylogenetic variance-covariance matrix
//' 
//' @author Adapted from Liam Revell's R function 'phylosig' {phytools}.
// [[Rcpp::export]]
double pagelLogLik(double lambda, NumericVector xr, NumericMatrix vcvr){
  
      unsigned int n = xr.size();
      arma::mat vcv(vcvr.begin(), n, n, false);
      arma::colvec x(xr.begin(), n, false);
      
      arma::colvec vcvDiag = vcv.diag();
      arma::mat vcvIdDiag = diagmat(vcvDiag);
      arma::mat vcvL = lambda * (vcv - vcvIdDiag) + vcvIdDiag;
      arma::mat vcvLInv = arma::inv(vcvL);
      double a = arma::accu(vcvLInv * x) / arma::accu(vcvLInv);
      arma::colvec xa = x - a;
      arma::rowvec xat = arma::trans(xa);
      double s = as_scalar(xat * vcvLInv * xa) / n;
      double logdetval;
      double logdetsign;
      arma::log_det(logdetval, logdetsign, s * vcvL);
      
      double logLik = as_scalar(-xat * (1/s * vcvLInv) * xa) / 2 - n * log(2 * pi)/2 - logdetval / 2;
      
      return logLik;
      
}


//' Mantel statistic
//' 
//' @param xr a matrix of traits.
//' @param Wr a matrix of weights.
// [[Rcpp::export]]
double mantelStat(NumericMatrix xr, NumericMatrix Wr){
  
  double ntip = xr.nrow();
  double nvar = xr.ncol();
  arma::mat W(Wr.begin(), ntip, ntip, false);
  arma::mat x(xr.begin(), ntip, nvar, false);
  
  arma::mat xcor = arma::cor(arma::trans(x));
  
  double trisize = ((ntip * ntip) - ntip) / 2;
  int idx = 0;
  arma::colvec triX(trisize);
  arma::colvec triW(trisize);
  
  for(unsigned int i = 0 ; i < ntip ; i++){
    for(unsigned int j = 0 ; j < ntip ; j++){
      if(i > j){
        triX(idx) = xcor(i, j);
        triW(idx) = W(i, j);
        idx = idx + 1;
      }
    }
  }
  triX = triX - arma::mean(triX);
  double res = arma::accu(triX % triW) / arma::accu(triW);
  
  return res;
}
