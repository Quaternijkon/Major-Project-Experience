package org.excellence.yunnanemploymentbackend.service;

import org.excellence.yunnanemploymentbackend.entity.EnterpriseInfo;
import org.excellence.yunnanemploymentbackend.entity.ReportedData;
import org.excellence.yunnanemploymentbackend.repository.EnterpriseInfoRepository;
import org.excellence.yunnanemploymentbackend.repository.EnterpriseRepository;
import org.excellence.yunnanemploymentbackend.repository.ProvinceRepository;
import org.excellence.yunnanemploymentbackend.repository.ReportedDataRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Optional;
import java.util.stream.Collectors;

@Service
public class EnterpriseService {
    @Autowired
    private EnterpriseInfoRepository enterpriseInfoRepository;

    @Autowired
    private ReportedDataRepository reportedDataRepository;

    @Autowired
    private ProvinceRepository provinceRepository;

    @Autowired
    private EnterpriseRepository enterpriseRepository;

    /**
     * 企业备案或上报数据
     *
     * @param provinceUserId   省的userId
     * @param enterpriseUserId 企业的userId
     * @param info             备案EnterpriseInfo或上报数据ReportedData
     * @param <T>              EnterpriseInfo或ReportedData
     * @return 操作是否成功
     */
    public <T> Boolean addNewInfo(String provinceUserId, String enterpriseUserId, T info) {
        final var wrappedProvince = provinceRepository.findById(provinceUserId);
        if (wrappedProvince.isEmpty()) {
            return false;
        }
        final var wrappedEnterprise = enterpriseRepository.findById(enterpriseUserId);
        if (wrappedEnterprise.isEmpty()) {
            return false;
        }
        final var province = wrappedProvince.get();
        final var enterprise = wrappedEnterprise.get();

        if (info.getClass() == EnterpriseInfo.class) {
            ((EnterpriseInfo) info).setStatus(EnterpriseInfo.Status.IN_REVIEWING); // 待审核
            final var insertedInfo = enterpriseInfoRepository.save(((EnterpriseInfo) info));
            // 省用户需要审核
            province.getEnterpriseInfoAwaitingReview().add(insertedInfo.getEnterpriseInfoId());
            provinceRepository.save(province);
            // 将备案信息加入到企业
            enterprise.setEnterpriseInfoId(insertedInfo.getEnterpriseInfoId());
        } else if(info.getClass() == ReportedData.class) {
            ((ReportedData) info).setStatus(ReportedData.Status.WAITING_APPROVED);
            final var insertedReport = reportedDataRepository.save((ReportedData) info);
            // 省审核
            province.getDataAwaitingReview().add(insertedReport.getReportedDataId());
            provinceRepository.save(province);
            // 加入企业自己
            enterprise.getReportedData().add(insertedReport.getReportedDataId());
        } else {
            return false;
        }
        enterpriseRepository.save(enterprise);
        return true;
    }

    /**
     * 获取企业所有历史上报信息
     * @param enterpriseUsdId 企业的userId
     * @return 历史上报信息列表
     */
    public List<ReportedData> getAllReportedData(String enterpriseUsdId) {
        return enterpriseRepository.findById(enterpriseUsdId)
                .map(enterprise -> enterprise.getReportedData().stream()
                        .map(integer -> reportedDataRepository.findById(integer).orElseGet(ReportedData::new)).
                        collect(Collectors.toList())).orElseGet(ArrayList::new);

    }
}
