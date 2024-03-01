package org.excellence.yunnanemploymentbackend.service;

import org.excellence.yunnanemploymentbackend.entity.Enterprise;
import org.excellence.yunnanemploymentbackend.entity.EnterpriseInfo;
import org.excellence.yunnanemploymentbackend.entity.Notice;
import org.excellence.yunnanemploymentbackend.entity.ReportedData;
import org.excellence.yunnanemploymentbackend.repository.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.function.Consumer;
import java.util.stream.Collectors;

@Service
public class ProvinceService {

    @Autowired
    private ProvinceRepository provinceRepository;

    @Autowired
    private EnterpriseRepository enterpriseRepository;

    @Autowired
    private EnterpriseInfoRepository enterpriseInfoRepository;

    @Autowired
    private ReportedDataRepository reportedDataRepository;

    @Autowired
    private NoticeRepository noticeRepository;

    /**
     * 查看所有已备案企业信息
     * @return 所有已备案企业的列表
     */
    public List<EnterpriseInfo> getAllRegisteredEnterprise() {
        return enterpriseRepository.findEnterprisesByIsRegisteredIsTrue().stream()
                .map(enterprise -> enterpriseInfoRepository.findEnterpriseInfoByEnterpriseUserId(enterprise.getUserId()))
                .collect(Collectors.toList());
    }

    public List<EnterpriseInfo> getAllEnterprise() {
        final var enterprises = enterpriseRepository.findAll();
        final var enterpriseList = new ArrayList<Enterprise>();
        for (var i : enterprises) {
            enterpriseList.add(i);
        }
        return enterpriseList.stream()
                .map(enterprise -> enterpriseInfoRepository.findEnterpriseInfoByEnterpriseUserId(enterprise.getUserId()))
                .collect(Collectors.toList());
    }

    /**
     * 查看各市已备案企业信息
     * @param cityUserId 所查询的市的userId
     * @return 当前市已备案的所有企业列表
     */
    public List<EnterpriseInfo> getRegisteredEnterpriseIn(String cityUserId) {
        return enterpriseRepository.findEnterprisesByIsRegisteredIsTrueAndLocatedCityIs(cityUserId).stream()
                .map(enterprise -> enterpriseInfoRepository.findEnterpriseInfoByEnterpriseUserId(enterprise.getUserId()))
                .collect(Collectors.toList());
    }

    /**
     * 获取所有待审核的企业上报数据
     * @return 待审核的企业上报数据的列表
     * @param provinceUserId 省的userId(方便多省扩展)
     */
    public List<ReportedData> getAllReportedDataAwaitingReview(String provinceUserId) {
        return provinceRepository.findById(provinceUserId)
                .map(province -> province.getDataAwaitingReview().stream()
                .map(reportedDataId -> reportedDataRepository.findById(reportedDataId).orElseGet(ReportedData::new))
                .collect(Collectors.toList())).orElseGet(ArrayList::new);
    }

    /**
     * 省审核通过上报的数据
     * @param reportedDataId 上报数据的Id
     * @return 操作是否成功
     */
    public Boolean approveReportedData(String provinceUserId, Integer reportedDataId) {
        try {
            if (removeReportedDataFromProvince(provinceUserId, reportedDataId)) {
                reportedDataRepository.findById(reportedDataId).ifPresentOrElse(reportedData -> {
                    reportedData.setStatus(ReportedData.Status.ALL_APPROVED);
                    reportedDataRepository.save(reportedData);
                }, () -> {});
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * 省拒绝通过企业上报的数据
     * @param provinceUserId 省的UserId
     * @param reportedDataId 上报数据的Id
     * @return 操作是否成功
     */
    public Boolean rejectReportedData(String provinceUserId, Integer reportedDataId) {
        try {
            if (removeReportedDataFromProvince(provinceUserId, reportedDataId)) {
                reportedDataRepository.findById(reportedDataId).ifPresentOrElse(reportedData -> {
                    reportedData.setStatus(ReportedData.Status.REJECTED_BY_PROVINCE);
                    reportedDataRepository.save(reportedData);
                }, () -> {});
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * 审核通过企业的备案信息
     * @param provinceUserId 省的UserId
     * @param enterpriseInfoId 备案信息的Id
     * @return 操作是否成功
     */
    public Boolean approveEnterpriseInfo(String provinceUserId, Integer enterpriseInfoId) {
        try {
            if (removeEnterpriseInfoFromProvince(provinceUserId, enterpriseInfoId)) {
                enterpriseInfoRepository.findById(enterpriseInfoId).ifPresentOrElse(enterpriseInfo -> {
                    enterpriseInfo.setStatus(EnterpriseInfo.Status.IS_APPROVED);
                    enterpriseInfoRepository.save(enterpriseInfo);
                }, () -> {});
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * 拒绝通过企业的备案信息
     * @param provinceUserId 省的UserId
     * @param enterpriseInfoId 备案信息的Id
     * @return 操作是否成功
     */
    public Boolean rejectEnterpriseInfo(String provinceUserId, Integer enterpriseInfoId) {
        try {
            if (removeEnterpriseInfoFromProvince(provinceUserId, enterpriseInfoId)) {
                enterpriseInfoRepository.findById(enterpriseInfoId).ifPresentOrElse(enterpriseInfo -> {
                    enterpriseInfo.setStatus(EnterpriseInfo.Status.IS_REJECTED);
                    enterpriseInfoRepository.save(enterpriseInfo);
                }, () -> {});
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * 修改企业的上报数据
     * @param reportedData 修改之后的上报数据
     * @return 操作是否成功
     */
    public Boolean modifyReportedData(ReportedData reportedData) {
        final var wrappedReportedData = reportedDataRepository.findById(reportedData.getReportedDataId());
        if (wrappedReportedData.isPresent()) {
            reportedDataRepository.save(reportedData);
            return true;
        } else {
            return false;
        }
    }

    /**
     * 发布通知
     * @param provinceUserId 省用户的UserId
     * @param notice 具体通知内容
     * @return 操作是否成功
     */
    public Boolean releaseNotice(String provinceUserId, Notice notice) {
        final var wrappedProvince = provinceRepository.findById(provinceUserId);
        if (wrappedProvince.isPresent())  {
            final var province = wrappedProvince.get();
            // 保存通知
            SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            notice.setDate(simpleDateFormat.format(new Date()));
            notice = noticeRepository.save(notice);
            // 记录到已发布通知中
            province.getReleasedNotices().add(notice.getNoticeId());
            // 把这个未读通知加入到所有企业的未读通知列表中
            for (final var enterprise: enterpriseRepository.findAll()) {
                enterprise.getUnreadNotices().add(notice.getNoticeId());
            }
            return true;
        } else {
            return false;
        }
    }

    /**
     * 移除省表中的某一待审核的ReportedData
     * @param provinceUserId 省用户的UserId
     * @param reportedDataId 待移除的reportedDataId
     * @return 操作是否成功
     */
    private Boolean removeReportedDataFromProvince(String provinceUserId, Integer reportedDataId) {
        try {
            final var wrappedProvince = provinceRepository.findById(provinceUserId);
            if (wrappedProvince.isPresent()) {
                final var province = wrappedProvince.get();
                province.getDataAwaitingReview().removeIf(integer -> integer.equals(reportedDataId));
                provinceRepository.save(province);
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * 移除省表中的某一待审核的EnterpriseInfo
     * @param provinceUserId 省用户的UserId
     * @param enterpriseInfoId 待移除的EnterpriseInfo
     * @return 操作是否成功
     */
    private Boolean removeEnterpriseInfoFromProvince(String provinceUserId, Integer enterpriseInfoId) {
        try {
            final var wrappedProvince = provinceRepository.findById(provinceUserId);
            if (wrappedProvince.isPresent()) {
                final var province = wrappedProvince.get();
                province.getEnterpriseInfoAwaitingReview().removeIf(integer -> integer.equals(enterpriseInfoId));
                provinceRepository.save(province);
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    public List<Notice> getAllNotice() {
        try{
            final List<Notice> notices = new ArrayList<>();
            for (var notice: noticeRepository.findAll()) {
                notices.add(notice);
            }
            return notices;
        } catch (Exception e) {
            return null;
        }
    }

    public Notice getNotice( Integer noticeId) {
        return noticeRepository.findById(noticeId).get();
    }
}
